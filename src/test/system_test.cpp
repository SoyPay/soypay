#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "init.h"
#include "miner.h"
#include "uint256.h"
#include "util.h"
#include <boost/test/unit_test.hpp>
#include "rpcclient.h"
#include "tx.h"
#include "wallet.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "./VmScript/VmScript.h"
#include "rpcserver.h"
#include "noui.h"
#include "ui_interface.h"
#include "SysTestBase.h"
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace boost;

class CSystemTest:public SysTestBase
{
public:
	CSystemTest() {
		StartServer();
	}

	~CSystemTest(){
		StopServer();
	}
private:
	void StartServer() {
//		int argc = 2;
//		char* argv[] = {"D:\\cppwork\\soypay\\src\\soypayd.exe","-datadir=d:\\bitcoin" };
//		SysTestBase::StartServer(argc,argv);
	}

	void StopServer() {
//		SysTestBase::StopServer();
	}

public:
	int GetBlockHeight()
	{
		return  (int)chainActive.Height();
	}

	bool GetBlockHash(int nHeight,uint256& blockHash)
	{
		if (nHeight < 0 || nHeight > chainActive.Height())
			return false;

		CBlockIndex* pblockindex = chainActive[nHeight];
		blockHash = pblockindex->GetBlockHash();
		return true;
	}

	bool IsTxConfirmdInWallet(const uint256& blockHash,const uint256& txHash)
	{
		auto itAccountTx = pwalletMain->mapInBlockTx.find(blockHash);
		if (pwalletMain->mapInBlockTx.end() == itAccountTx)
			return false;

		for (const auto &item :itAccountTx->second.mapAccountTx) {
			if (txHash == item.first) {
				return true;
			}
		}
		return false;
	}

	bool IsTxUnConfirmdInWallet(const uint256& txHash) {
		for (const auto &item : pwalletMain->UnConfirmTx) {
			if (txHash == item.first) {
				return true;
			}
		}
		return false;
	}

	bool IsTxInMemorypool(const uint256& txHash) {
		for (const auto& entry : mempool.mapTx) {
			if (entry.first == txHash)
				return true;
		}

		return false;
	}

	bool GetTxIndexInBlock(const uint256& txHash, int& nIndex) {
		CBlockIndex* pindex = chainActive.Tip();
		CBlock block;
		if (!ReadBlockFromDisk(block, pindex))
			return false;

		block.BuildMerkleTree();
		std::tuple<bool,int> ret = block.GetTxIndex(txHash);
		if (!std::get<0>(ret)) {
			return false;
		}

		nIndex = std::get<1>(ret);
		return true;
	}

	bool GetRegScript(map<string, string>& mapRegScript) {
		CRegID regId;
		vector<unsigned char> vScript;

		if (pScriptDBTip == nullptr)
			return false;

		assert(pScriptDBTip->Flush());

		int nCount(0);
		if (!pScriptDBTip->GetScriptCount(nCount))
			return false;

		if (!pScriptDBTip->GetScript(0, regId, vScript))
			return false;

		string strRegID = HexStr(regId.GetVec6());
		string strScript = HexStr(vScript.begin(), vScript.end());
		mapRegScript.insert(make_pair(strRegID, strScript));

		while (pScriptDBTip->GetScript(1, regId, vScript)) {
			strRegID = HexStr(regId.GetVec6());
			strScript = HexStr(vScript.begin(), vScript.end());
			mapRegScript.insert(make_pair(strRegID, strScript));
		}

		return true;
	}

	bool CheckRegScript(const string& strRegID,const string& strPath) {
		map<string, string> mapRegScript;
		if (!GetRegScript(mapRegScript)) {
			return false;
		}

		string strFileData;
		if (!GetFileData(strPath,strFileData)) {
			return false;
		}

		for (const auto& item:mapRegScript) {
			if (strRegID == item.first) {
				if (strFileData == item.second) {
					return true;
				}
			}
		}

		return false;
	}

	bool GetFileData(const string& strFilePath, string& strFileData) {
		FILE* file = fopen(strFilePath.c_str(), "rb+");
		if (!file) {
			return false;
		}

		long lSize;
		size_t nSize = 1;
		fseek(file, 0, SEEK_END);
		lSize = ftell(file);
		rewind(file);

		// allocate memory to contain the whole file:
		char *buffer = (char*) malloc(sizeof(char) * lSize);
		if (buffer == NULL) {
			return false;
		}

		if (fread(buffer, 1, lSize, file) != lSize) {
			if (buffer)
				free(buffer);
			throw runtime_error("read script file error");
		}

		CVmScript vmScript;
		vmScript.Rom.insert(vmScript.Rom.end(), buffer, buffer + lSize);
		CDataStream ds(SER_DISK, CLIENT_VERSION);
		ds << vmScript;

		vector<unsigned char> vscript;
		vscript.assign(ds.begin(), ds.end());

		if (file)
			fclose(file);
		if (buffer)
			free(buffer);

		strFileData = HexStr(vscript);
		return true;
	}

	bool GetHashFromCreatedTx(const Value& valueRes,string& strHash)
	{
		if (valueRes.type() == null_type) {
			return false;
		}

		const Value& result = find_value(valueRes.get_obj(), "hash");
		if (result.type() == null_type){
			return false;
		}

		strHash = result.get_str();
		return true;
	}

	bool GetTxOperateLog(const uint256& txHash, vector<CAccountOperLog>& vLog) {
		if (!GetTxOperLog(txHash, vLog))
			return false;

		return true;
	}

private:
	boost::thread* pThreadShutdown;
};

BOOST_FIXTURE_TEST_SUITE(system_test,CSystemTest)
BOOST_FIXTURE_TEST_CASE(reg_test,CSystemTest)
{
	int nOldBlockHeight = 0;
	int nNewBlockHeight = 0;
	int nTimeOutHeight = 5;
	int nFee = 100000;
	uint64_t nOldMoney = 0;
	uint64_t nNewMoney = 0;
	string strTxHash;
	string strFileName("RegScriptTest.bin");
	string strAddr("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA");

	vector<map<int,string> >vDataInfo;
	vector<CAccountOperLog> vLog;
	for (int i = 0; i < nTimeOutHeight; i++) {
		//0:����ע��ű�����
		Value valueRes = RegisterScriptTx(strAddr,strFileName , nTimeOutHeight, nFee);
		BOOST_CHECK(GetHashFromCreatedTx(valueRes,strTxHash));

		//1:�ڿ�
		nOldMoney = GetFreeMoney(strAddr);
		nOldBlockHeight = GetBlockHeight();
		BOOST_CHECK(GenerateOneBlock());
		nNewBlockHeight = GetBlockHeight();

		//2:ȷ��Ǯ�Ѿ��۳�
		BOOST_CHECK(nNewBlockHeight = nOldBlockHeight + 1);
		nNewMoney = GetFreeMoney(strAddr);
		BOOST_CHECK(nNewMoney == nOldMoney - nFee);

		//3:ȷ�Ͻű��˺��Ѿ�����
		int nIndex = 0;
		BOOST_CHECK(GetTxIndexInBlock(uint256(strTxHash), nIndex));
		CRegID regID(nNewBlockHeight, nIndex);
		BOOST_CHECK(IsScriptAccCreated(HexStr(regID.GetVec6())));
		BOOST_CHECK(!IsScriptAccCreated("000000000000"));

		//4:���Ǯ�������ȷ�Ͻ������Ƿ��д˱ʽ���
		uint256 blockHash;
		BOOST_CHECK(GetBlockHash(nNewBlockHeight, blockHash));
		BOOST_CHECK(IsTxConfirmdInWallet(blockHash, uint256(strTxHash)));

		//5:ͨ��listregscript ��ȡ�����Ϣ��һһ�˶ԣ����Ƿ�������һ��
		string strPath = SysCfg().GetDefaultTestDataPath() + strFileName;
		BOOST_CHECK(CheckRegScript(HexStr(regID.GetVec6()), strPath));

		//6:Gettxoperationlog ��ȡ����log���鿴�Ƿ���ȷ
		BOOST_CHECK(GetTxOperateLog(uint256(strTxHash), vLog));
		BOOST_CHECK(1 == vLog.size() && 1 == vLog[0].vOperFund.size() && 1 == vLog[0].vOperFund[0].vFund.size());
		BOOST_CHECK(strAddr == vLog[0].keyID.ToAddress());
		BOOST_CHECK(vLog[0].vOperFund[0].operType == MINUS_FREE && vLog[0].vOperFund[0].vFund[0].value == nFee);

		map<int,string> mapData;
		mapData.insert(make_pair(nIndex,strTxHash));
		vDataInfo.push_back(std::move(mapData));
	}

	for(int i = vDataInfo.size()-1;i>=0;i--) {
		map<int,string> mapData = vDataInfo[i];
		BOOST_CHECK(1 == mapData.size());

		int nTxIndex = mapData.begin()->first;
		string strTxHash = mapData.begin()->second;
		uint256 txHash(strTxHash);

		nOldBlockHeight = GetBlockHeight();
		nOldMoney = GetFreeMoney(strAddr);

		//8:�ع�
		BOOST_CHECK(DisConnectBlock(1));

		//9.1:����˻��������Ƿ����
		nNewMoney = GetFreeMoney(strAddr);
		nNewBlockHeight = GetBlockHeight();
		BOOST_CHECK(nOldBlockHeight - 1 == nNewBlockHeight);
		BOOST_CHECK(nNewMoney-nFee == nOldMoney);

		//9.2:���ű��˻��Ƿ�ɾ��
		CRegID regID(nOldBlockHeight, mapData.begin()->first);
		BOOST_CHECK(!IsScriptAccCreated(HexStr(regID.GetVec6())));

		//9.3:�����Ƿ��Ѿ��Ѿ��ŵ�Ǯ����δȷ�Ͻ�����
		BOOST_CHECK(IsTxUnConfirmdInWallet(txHash));

		//9.4:��齻���Ƿ���mempool��
		BOOST_CHECK(IsTxInMemorypool(txHash));

		//9.5:���operationlog �Ƿ�������»�ȡ
		BOOST_CHECK(!GetTxOperateLog(txHash, vLog));
	}

	//��ջ���
	ResetEnv();
	nNewBlockHeight = GetBlockHeight();
	BOOST_CHECK(0 == nNewBlockHeight);
}

BOOST_FIXTURE_TEST_CASE(author_test,CSystemTest)
{

}
BOOST_AUTO_TEST_SUITE_END()

