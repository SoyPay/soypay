#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "miner.h"
#include "uint256.h"
#include "util.h"
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>
#include "VmScript/VmScriptRun.h"
#include "VmScript/CVir8051.h"
#include "VmScript/TestMcu.h"
#include "json/json_spirit_writer_template.h"
#include "rpcclient.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
using namespace std;
using namespace boost;
using namespace json_spirit;

extern Object CallRPC(const string& strMethod, const Array& params);
extern int TestCallRPC(std::string strMethod, const std::vector<std::string> &vParams, std::string &strRet);
extern void GetAccountInfo(char *address);
extern void GenerateMiner();

void CreateRegScriptdbTx()
{
	int argc = 7;
	char* path = "D:\\bitcoin\\data\\testrollback.bin";
	string message =path;
	message +=" not exitst";
		BOOST_CHECK_MESSAGE(boost::filesystem::exists(path),message);
	char *argv[7] =
			{ "rpctest", "registerscripttx", "5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG","0",
					path,
					"1000000", "2" };
	CommandLineRPC(argc, argv);
}
void CreateContactTx(int param)
{
	int argc = 8;
	std::vector<std::string> vInputParams;
	vInputParams.clear();
	vInputParams.push_back("010000000100");
	vInputParams.push_back(
			"[\"5yNhSL7746VV5qWHHDNLkSQ1RYeiheryk9uzQG6C5d\"]");
	char buffer[3] = {0};
	sprintf(buffer,"%02x",param);
	vInputParams.push_back(buffer);
	vInputParams.push_back("1000000");
	vInputParams.push_back("10");
	std::string strReturn("");
	TestCallRPC("createcontracttx", vInputParams, strReturn);
	cout<<strReturn<<endl;
	return ;
}
void disblock1()
{
	int argc = 3;
	char *argv[3] = { "rpctest", "disconnectblock", "1" };
//	sprintf(argv[2], "%d", number);
	CommandLineRPC(argc, argv);
}
BOOST_AUTO_TEST_SUITE(test_rollback)

BOOST_AUTO_TEST_CASE(db_fun)
{
	CreateRegScriptdbTx();
	GenerateMiner();
	cout << "1" << endl;
	CreateContactTx(1);    //�����ű�����
	GenerateMiner();
	cout << "2" << endl;
	CreateContactTx(2);;   //�޸Ľű�����
	GenerateMiner();

	cout << "3" << endl;
	CreateContactTx(3);    //ɾ���ű�����
	GenerateMiner();

	cout << "4" << endl;
	disblock1();           //ɾ��1��block
	cout << "41" << endl;
	CreateContactTx(4);    //checkɾ���Ľű��Ƿ�ָ�
	GenerateMiner();

	cout << "5" << endl;
	disblock1();
	disblock1();
	CreateContactTx(5);    //check�޸ĵĽű������Ƿ�ָ�
	GenerateMiner();

	cout << "6" << endl;
	disblock1();
	disblock1();
	CreateContactTx(6);   //check�����Ľű������Ƿ�ָ�
	GenerateMiner();

}
BOOST_AUTO_TEST_SUITE_END()
