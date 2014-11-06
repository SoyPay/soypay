// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
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
using namespace std;
using namespace boost;
using namespace json_spirit;

extern Object CallRPC(const string& strMethod, const Array& params);
extern int TestCallRPC(std::string strMethod, const std::vector<std::string> &vParams, std::string &strRet);
extern void GetAccountInfo(char *address);
extern void GenerateMiner();
	//
//	string strPrint;
//	int nRet;
//	Array params = RPCConvertValues(strMethod, vParams);
//
//	Object reply = CallRPC(strMethod, params);
//
//	// Parse reply
//	const Value& result = find_value(reply, "result");
//	const Value& error = find_value(reply, "error");
//
//	if (error.type() != null_type) {
//		// Error
//		strPrint = "error: " + write_string(error, false);
//		int code = find_value(error.get_obj(), "code").get_int();
//		nRet = abs(code);
//	} else {
//		// Result
//		if (result.type() == null_type)
//			strPrint = "";
//		else if (result.type() == str_type)
//			strPrint = result.get_str();
//		else
//			strPrint = write_string(result, true);
//	}
//	strRet = strPrint;
//	BOOST_MESSAGE(strPrint);
//	//cout << strPrint << endl;
//	return nRet;
//}
//static void GetAccountInfo(char *address) {
//	int argc = 3;
//	char *argv[3] = { "rpctest", "getaccountinfo", address };
//	CommandLineRPC(argc, argv);
//
//}
//static void GenerateMiner() {
//	int argc = 3;
//	char *argv[3] = { "rpctest", "setgenerate", "true" };
//	CommandLineRPC(argc, argv);
//}


std::string TxHash("");

void GetAccountState1() {
	GetAccountInfo("5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG");
}
void CreateRegScriptTx1() {
	cout <<"CreateRegScriptTx1" << endl;
	int argc = 6;
	char *argv[6] =
			{ "rpctest", "registerscripttx", "5zQPcC1YpFMtwxiH787pSXanUECoGsxUq3KZieJxVG",
					"00200005300000000000000002202200000000000000000000000002202200200e0fb0e409000f0760780470790010800020f00a30d80fc0d90fa07a00007b00f0900000620780770790010800150e40930a30ad0820ae08308a08208b0830f00a30aa0820ab08308d08208e0830d80e90d90e701200901401200f03b0750d00000750810bf07501000007501100f00200001704307507207206506e07402007406907002006806107306803a00004706507405407806906e06606f03a00003507a05105006304303105907004604d07407707806904803703803707005305806106e05504504306f04707307805507103304b05a06906504a07805604700004706507404106306306f07506e07405007506206c06906306b06507903a00005107506507207904106306306f07506e07404206106c06106e06306503a0000010000000000e00fa0a30e00fb0a30e00fc0a30e00fd0220ca0c00e00e60f00a30080da0fa0d00e00fa0220ca0c00e00e00a30c50820cc0c50820c50830cd0c50830f00a30c50820cc0c50820c50830cd0c50830da0e60d00e00ca0220b90000100bb0000080e808a0f00840f80aa0f00220e40fb0c80fa0220eb07002207b0100c80330c80c90330c90330500070c309a0c30db0f208000609a05000102a0db0ea0c80330f40c80c90330f40c90fa0220750f00080e40c80330c80c90330c90330c909a0c909b0500040c902a0c903b0d50f00ec0fb0c80330f40c80e40c90fa0220250100100af0080f501004000c0150110800080f50100400020150110d20af0220000c00d00250100100af0080f501005000c0050110800080f50100500020050110d20af0d00d00220250100f50820100af0080f501004000c0150110800080f50100400020150110d20af0850110830220250100f50820e40350110f50830220250100f80e40350110f90220250100fa0e40350110fb0220250100fc0e40350110fd0220250100c50820c00e00e50110340ff0c50830c00e00e50100c30950820240f90100af0080850830110850820100800080850830110850820100d20af0ce0f00a30e50200f00a30780080e60080f00a30de0fa0ef0f00a30e50810240fb0f80e60080f00a30e60080f00a30080080e60080f00a30e60080f00a30150810150810d00e00fe0d00e00f80150810150810e80c00e00ee0c00e00220850110830850100820e00a30fe0e00a30f50200780080e00a30f60080df0fa0e00a30ff0e00a30c00e00e00a30c00e00e00a30c00e00e00a30c00e00100af0080850820100850830110800080850820100850830110d20af0d00830d00820220740020800000c00e00f400401200109c0d00e00120000e40220740020800000cc0c00e00ed0c00e00e50100c309c0cc0ad01105000101d0100af00608c01008d01108000608c01008d0110d20af0120000f10d00e00fd0d00e00fc0220740f80120010d90e90fe0ee0900fb0fe0f001200000807f00100200203f0c00820c00830ea0900fb0fe0f00e40cb0fa0ea0900fb0ff0f00120000120d00830d00820220c00820c008308a08208b0830e00fa0a30e00fb0d00830d00820220ea02c0f80eb03d0f90c30e809a0e909b0400070c30e809c0e909d0500040d20f00800020c20f00a20f00220740f30120010d90740fc01200106b0850100820850110830ec0f00a30ed0f008a00a08b00b0750080fe0750090f70aa0080ab0090120020ea08a00c08b00d0ae00c0af00d0850100820850110830e002e0f80a30e003f0f90e80240020f80500010090740020120010b70e80f00a30e90f00850100820850110830e00240020fc0a30e00340000fd0ee0fa0ef0fb0120020fc04001407c00207d0000850100820850110830e00fa0a30e00fb0120020fc0500050790000120020bd07500c00207500d00007800c01200207f0740040120010d10aa0080ab00901200e05907400201200108107500c00207500d00007800c01200207f0740020120010d10e500802e0f80e500903f0f90e80240020fa0e40390fb01200e0590740020120010810850100820850110830120020900ac00a0ad00b0e500802e0f80e500903f0f90e80240040fa0e40390fb01200e05907400201200108107400401200108107f00600200203f0740f50120010d90ea0fe0eb0ff08c00808d00907500a00007500b00207800a01200207f07c00007d00007a0fe07b0f701200e0d10740020120010810ee04f07000d07c00107d00007a00007b0000120030190800120ee0fa0ef0fb01200f01f0ea0fc0eb0fd0ee0fa0ef0fb01200301907400b0120010b70e00fc0a30e00fd0aa0080ab00901200301907a00907b0000120020d107f00400200203f0740f50120010d90740fc01200106b0ea0fe0eb0ff07500800007500900207800801200207f07c00007d00007a0fe07b0f701200e0d10740020120010810ee0fa0ef0fb01200f01f08a00808b0090e50080240010f50080500020050090e40f500a0f500b0ac0080ad0090ee0fa0ef0fb01200301907a00b07b0000120020d107a0fe07b0f708a08208b0830e00640040700040a30e006400006000a07a00007b00007c00007d00008004107c00007d00007e00007f0000850100820850110830ec0f00a30ed0f00a30ee0f00a30ef0f007500800407500900007800801200207f0ea0240020fc0e403b0fd0740020120010c901200e0590740020120010810850100820850110830120000d807400401200108107f00400200203f0740f50120010d90740fc01200106b0ea0fe0eb0ff07500800007500900207800801200207f07c00007d00007a0fe07b0f701200e0d10740020120010810ee0fa0ef0fb01200f01f08a00808b0090e50080240010f50080500020050090e40f500a0f500b0ac0080ad0090ee0fa0ef0fb01200301907a00c07b0000120020d107a0fe07b0f708a08208b0830e00640040700040a30e006400006000a07a00007b00007c00007d00008004107c00007d00007e00007f0000850100820850110830ec0f00a30ed0f00a30ee0f00a30ef0f007500800407500900007800801200207f0ea0240020fc0e403b0fd0740020120010c901200e0590740020120010810850100820850110830120000d807400401200108107f00400200203f0740f30120010d90ea0fe0eb0ff08c00c08d00d07500800007500900207800801200207f07c00007d00007a0fe07b0f701200e0d10740020120010810ee0fa0ef0fb01200f01f08a00808b0090e50080240010f80e40350090f90e40fa0fb08800808900908a00a08b00b0ac0080ad0090ee0fa0ef0fb01200301907a00a07b0000120020d107e0fe07f0f708e08208f0830e00f80a30e00f90c30e500c0980e500d0990500040c30020060de08e08208f0830120020900ee0240020fc0e403f0fd07400f0120010b70e00fa0a30e00fb01200e05907400201200108108e08208f0830e00f80a30e00f90ee0280f80ef0390f90e80240020fe0e40390ff08e08208f0830e00f80a30e00f907a01407b00001200101208a00808b0090e500804500907000e08e08208f0830c30e00940150a30e00940000400030c308002108e08208f0830120020900ee0240020fc0e403f0fd0740110120010b70e00fa0a30e00fb01200e0590740020120010810d307f00600200203f0740f30120010d90ea0fe0eb0ff08c00c08d00d07500800007500900207800801200207f07c00007d00007a0fe07b0f701200e0d10740020120010810ee0fa0ef0fb01200f01f08a00808b0090e50080240010f80e40350090f90e40fa0fb08800808900908a00a08b00b0ac0080ad0090ee0fa0ef0fb01200301907a00d07b0000120020d107a0fe07b0f707400d0120010b70c00820c008308a08208b0830e00f80a30e00f90d00830d00820c30e00980a30e009904001e08a08208b0830e00640410700040a30e006400006001208a08208b0830e00640210700040a30e00640000600030c308001b08a08208b0830120020900ea0240020fc0e403b0fd0aa00c0ab00d01200e0590740020120010810d307f00600200203f0740f30120010d90ea0fe0eb0ff08c00c08d00d07500800007500900207800801200207f07c00007d00007a0fe07b0f701200e0d10740020120010810ee0fa0ef0fb01200f01f08a00808b0090e50080240010f80e40350090f90e40fa0fb08800808900908a00a08b00b0ac0080ad0090ee0fa0ef0fb01200301907a00e07b0000120020d10780fe0790f70880820890830e00640080700040a30e00640000600030c308001b0880820890830120020900e80240020fc0e40390fd0aa00c0ab00d01200e0590740020120010810d307f00600200203f0740f70120010d90740fc01200106b0850100820850110830ea0f00a30eb0f00a30ec0f00a30ed0f007500800007500900207800801200207f07c00007d00007a0fe07b0f701200e0d107400201200108107c00407d0000aa0100ab01101200301907a01107b0000120020d107a0fe07b0f707400f0120010b70c00820c008308a08208b0830e00f80a30e00f90d00830d00820c30e00980a30e00990500030c308002108a08208b0830120020900ea0240020fc0e403b0fd07400f0120010b70e00fa0a30e00fb01200e0590740020120010810d307400401200108107f00200200203f0740f70120010d908a00808b0090ec0fe0ed0ff07a01a07b0000120020d107a0fe07b0f708a08208b0830e00f80a30e00f90c30ee0980ef0990500030c308001b08a08208b0830120020900ea0240020fc0e403b0fd0aa0080ab00901200e0590740020120010810d307f00200200203f00200b0700740b401200106b09000f07607402c0120010d10740200120000f109000f09607400c0120010d10740200120000f107c02007d00007400c0120010c90120080c607500802007500900007800801200207f07402e0120010c108800808900907800801200207f09000f0720120000d801200802e07400401200108107500801e07500900007800801200207f07c00007d00f07a00007b00001200400707400201200108107500802007500900007800801200207f07402e0120010d107a00007b00001200400707400201200108107400c0120010c901200406f08a00808b00908c00a08d00b08500800a08500900b07400c0120010c901200502c08a00c08b00d08c00e08d00f08500c00808500d0090aa0080ab0090120090110850100820850110830ea0f00a30eb0f00aa0080ab0090120090110740020120010b70ea0f00a30eb0f007800801200207f07c00007d0000740020120010b70e00fa0a30e00fb01200e0d107400201200108107800a01200207f07c00007d0000740040120010b70e00fa0a30e00fb01200e0d10740020120010810e500a0240020f500c0e403500b0f500d07800c01200207f0740020120010b70740040120020940e50080240020fc0e40350090fd0740120120010c90120050e907400601200108107500c01407500d00007800c01200207f07c01207d00f07a00007b0000120040070740020120010810e50080240020f500805000200500907800801200207f0740020120010b70e00fc0a30e00fd07a00007b0000120040070740020120010810e500a0240020f50080e403500b0f500907800801200207f0740040120010b70e00fc0a30e00fd07a00007b00001200400707400201200108107500801d07500900f07a04107b00001200901108a00a08b00b0ae00a0af00b07500a04107500b00007800a01200207f0ee0fc0ef0fd0aa0080ab0090120060e307400201200108107500a01407500b00007800a01200207f07c04807d00f07a00007b00001200400707400201200108107500a04107500b00007800a01200207f0ee0fc0ef0fd07a00007b0000120040070740020120010810740040120010d10aa0080ab00901200709f07500801407500900007800801200207f07c05d07d00f07a00007b00001200400707400201200108107500800807500900007800801200207f0740060120010d107a00007b0000120040070740020120010810790010120020bd07a00007b00007404c0120010810220740f40120010d90ea0240020fe0e403b0ff0c30ea09e0eb09f04000300200c06f0c30ee0940040ef09400005000c07e00407f00008000601200d00b01200d01309000f0b80e00fa0a30e00fb0ea04b07002c07a0ba07b00f01200c0ff07000300200c05d0880820890830c30e009e0a30e009f05005301200d00b0800e70880820890830c30e009e0a30e009f05004201200d00b01200c0ff0700ec09000f0b80e00f80a30e00f50830880820e00f80a30e00f90e80fc0e90fd07a0ba07b00f08000301200d00b08a08208b0830e00f80a30e00f90ec0680700020ed0690600570880820890830c30e009e0a30e009f0400e00ea0f80eb0f90e80490600550880820890830e00fa0a30e00fb0ea0240020fc0e403b0fd08a08208b0830e00f50080a30e00f50090e50080240fc0f500a0e50090340ff0f500b0c30e500a09e0e500b09f05002b08c08208d0830e00fc0a30e00fd0880820890830ec0f00a30ed08005f01200e09608a00808b0090a80080a90090e804906000300200b09507a00007b0000800780ea02e0f500a0eb03f0f500b0880820890830e500a0f00a30e500b0f008c08208d0830e00f500a0a30e00f500b01200c0f20a30a30e500a0f00a30e500b0f00e50080c309e0f50080e500909f0f500901200c0f20e50080f00a30e50090f008a08208b0830ee0f00a30ef0f008808208908301200d0030600140e80240020f80500010090880820890830e00fc0a30e00fd0ec04d07000407800007900009000f0b80e80f00a30e90f00ea0240020fa05000100b07f00500200203f0880820890830e00fc0a30e00f508308c08202208a08208b0830e00f80a30e00f90e80490220e80240020fa0e40390fb0220740f30120010d90ea04b07000300200e04a0ea0240fe0fa0eb0340ff0fb08a08208b0830e00fe0a30e00ff0c30ee0940040ef09400005000300200e04a09000f0ba0e00fc0a30e00fd0ea0240020f50080e403b0f50090ec04d06000b0ea0f80eb0f90c30e809c0e909d0500150850080820850090830ec0f00a30ed0f009000f0ba00200d0ef0ac0820ad0830ec0240020f500a0e403d0f500b08500a0820f50830e00f500c0a30e008500c0820f50830e50820450830600090c30e50820980e50830990400d501200e04f0ec0280f80ed0390f90c30ea0980eb09905000300200e04a0ea0680700020eb06907001108c08208d0830e002e0f00a30e003f0f00ec0fa0ed0fb08003008500a08208500b08301200e0530e804906000d0ea02e0fc0eb03f0fd0c30e809c0e909d04006c0850080820850090830e80f00a30e90f008500a08208500b0830ea0f00a30eb0f00ea0240020f80e403b0f90e80fc0e90fd01200e04f0e804906004408a08208b0830e00fe0a30e00ff0ea02e0fe0eb03f0ff0e806e0700020e906f07002d09000f0b80e40f00a30f00880820890830e00fe0a30e00ff08a08208b0830e002e0f00a30e003f0f00880820890830a30a301200e05308c08208d0830e80f00a30e90f007f00600200203f08c08208d0830e00f80a30e00f90220740f80120010d90ea0f80eb0f90740080120010b70e00fe0a30e008001f08c08208d0830e00880820890830f00a30a80820a908308c08208d0830a30ac0820ad0830ee0240ff01e0ef0340ff0ff0ee04f0700dc07f00100200203f0c00820c008309000f0b60e00f80a30e00f90e804906000607a00007b00008002007a0bc07b00f09000f0b60ea0f00a30eb0f00740bb0c30940bc07401009400f0f908a08208b0830740ff0f00a30e90f00d00830d00820220c00820c00830850100820850110830e00f80a30e00f90e804906001208a08208b0830ec0f00a30e80240ff0180e90340ff0f90480700f20d00830d00820220740f80120010d90740fe01200106b0ea0fe0eb0ff0850100820850110830ee0f00a30ef0f00aa0100ab01107900101200f03e0800ea0c00820c008308a08208b0830800010a30e00700fc0e50820c309a0fa0e508309b0fb0d00830d0082022002000014022",
					"1000000", "2" };
	CommandLineRPC(argc, argv);
}
bool CreateTx()
{
	int argc = 8;
	std::vector<std::string> vInputParams;
	vInputParams.clear();
	vInputParams.push_back("010000000100");
	vInputParams.push_back(
			"[\"5yNhSL7746VV5qWHHDNLkSQ1RYeiheryk9uzQG6C5d\"]");
	vInputParams.push_back("0b434b430046003531303030303030300000000000000000000000");
	vInputParams.push_back("100000");
	vInputParams.push_back("10");
	std::string strReturn("");
	if (TestCallRPC("createcontracttx", vInputParams, strReturn) > 0) {
		vInputParams.clear();
		cout << "create secure tx succeed1:"<<strReturn<< endl;
		TxHash = strReturn;
	}
	return false;
}
void ListRegScript1() {
	//cout << "listRegScript" << endl;
	int argc = 2;
	char *argv[2] = { "rpctest", "listregscript" };
	CommandLineRPC(argc, argv);
}
BOOST_AUTO_TEST_SUITE(VM_fun)

BOOST_AUTO_TEST_CASE(Gloal_fun)
{
	//cout << "=====================init account info ========================" << endl;
	GetAccountState1();
	CreateRegScriptTx1();
	GenerateMiner();
	cout << "=====================create tx 1========================" << endl;
	ListRegScript1();
	CreateTx();
	GenerateMiner();
//	GetAccountState1();
}
BOOST_AUTO_TEST_SUITE_END()
