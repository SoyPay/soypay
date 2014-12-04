#include "rpcserver.h"
#include "rpcclient.h"
#include "util.h"
#include <boost/test/unit_test.hpp>
#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost;
using namespace json_spirit;

map<string, string> mapDesAddress[] = {
        boost::assign::map_list_of
        ("000000000900",	"mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA")
        ("000000000500",	"mv2eqSvyUA4JeJXBQpKvJEbYY89FqoRbX5")
        ("000000000300",	"mhVJJSAdPNDPvFWCmQN446GUBPzFm8aN4y")
        ("000000000800",	"n4muwAThwzWvuLUh74nL3KYwujhihke1Kb")
        ("000000000700",	"mfu6nTXP9LR9mRSPmnVwXUSDVQiRCBDJi7")
        ("000000000400",	"moZJZgsGFC4qvwRdjzS7Bj3fHrtpUfEVEE")
        ("000000000100",	"mjSwCwMsvtKczMfta1tvr78z2FTsZA1JKw")
        ("000000000600",	"mrjpqG4WsyjrCh8ssVs9Rp6JDini8suA7v")
        ("000000000200",	"mw5wbV73gXbreYy8pX4FSb7DNYVKU3LENc"),

        boost::assign::map_list_of
        ("1-70",	"mm8f5877wY4u2WhhX2JtGWPTPKwLbGJi37")
        ("1-62",	"n31DG5wjP1GcKyVMupGBvjvweNkM75MPuR")
        ("1-48",	"mkeict2uyvmb4Gjx3qXh6vReoTw1A2gkLZ")
        ("1-63",	"mqwRHqjQZcJBqJP46E256Z2VuqqRAkZkKH")
        ("1-58",	"mvV1fW4NMv9MGoRwHw583TDi8gnqqjQovZ")
        ("1-46",	"mzGkgfAkWtkQ4AP2Ut4yiAzCNx6EDzWjW8")
        ("1-47",	"n3wo9Ts6AUmHdGM1PixpRnLRFrG5G8a5QA")
        ("1-43",	"mxrmM6qNswgZHmp1u2HTu2soncQLkud7tF")
        ("1-45",	"mmBBV47uFguukjceTXkPsB3izndht2YXx7"),

        boost::assign::map_list_of
        ("1-61",	"n3eyjajBMwXiK56ohkzvA2Xu53W9E6jj8K")
        ("1-37",	"muDs5TAdk6n8rSyLdq6HTzBkJT2XxPF1wP")
        ("1-69",	"mxZqVtfao3A6dbwymKtn6oE4GacXoJNsac")
        ("1-41",	"mspw67fn4KGwUrG9oo9mvLJQAPrTYwxQ6w")
        ("1-38",	"mvNmNnB98GDSeYqg2jH2gSU557XEivs3N5")
        ("1-72",	"mjEGztB67nfscqSg5ryUGtzyTGwwEASZeQ")
        ("1-50",	"mmCvt8WZzF27VGBMWWVkED3vsDRdpnigGV")
        ("1-71",	"miUVkZNCDaKLTveqT3uWcy8kkpkA94gNvS")
        ("1-44",	"mxx4MohV2ZfifQiZnmU4yVUVf2QUVM2grx"),

		boost::assign::map_list_of
        ("1-65",	"mjuZWVqVQ2cmFoB8pJRj7XWVCPkeoiWJAq")
        ("1-57",	"n2tTaaF8xoWWYvaxSDkfQP5GeEcCCsjq1t")
        ("1-56",	"mw1XUknDsVtb68BUJNj25rKAikYG8qELHJ")
        ("1-60",	"mgE3hASaGCRPxJdZruAsydr2ygQz2UBWZM")
        ("1-42",	"n1vNXyu2GNypJGdZYxzCBCeQFVt1Fd42Qn")
        ("1-55",	"n4Cti65cSeufvfxStKUozHNGX3fQSHsDe5")
        ("1-54",	"mnZUhyb83ZTQWc9TXXFfhjJEu65q4cFj4S")
        ("1-59",	"muGiULSeqi2FQ2ypzU7aP8Uu1SWC5kRBki")
        ("1-39",	"mrMFs4kk8sqZ7iE8DquqPLL8udyGNDUZ8T"),

        boost::assign::map_list_of
        ("1-53",	"mogX7FTZ9Yuu6gYscKaEf2oxroeRuNDi76")
        ("1-66",	"mgs1mDsaXuj16aJ5YMHqLx7xsQ88snsZmB")
        ("1-68",	"mzUKrawp7a7LNB7D7kKzKEpgAStsAAHz18")
        ("1-64",	"miNou7awKXUPN9wbzVP32zTXcWvPsZBpYg")
        ("1-51",	"mnnd1QQx2dM5yfp1j8Vp7Dcq7BhiS6bNEQ")
        ("1-49",	"muS2Nxtva88d45uN6up7WeHszi3oWAcadK")
        ("1-67",	"mw8yB7Pp7GYiDHhLQT2GNsLc439rfJ3Fai")
        ("1-52",	"miRVDrwxtJJh4XnZFnYR6YbdqpAuirVDzZ")
        ("1-40",	"mvqUh3LR4R7cDWfw4AW7mRUSxfZbvonQ8v")};


string sendValues[2][10] = {
		{"3B9ACA00","77359400","B2D05E00","EE6B2800","12A05F200","165A0BC00","1A13B8600","1DCD65000","218711A00","2540BE400"},
		{"1000000000","2000000000","3000000000","4000000000","5000000000","6000000000","7000000000","8000000000","9000000000","10000000000"}
	};


class CMiningTest {
public:
	//��ʼ�����л���������Block��Ϣ
	CMiningTest() {

	}
	~CMiningTest() {};

};


class CSendItem{
private:
	string m_strRegId;
	string m_strAddress;
	string m_strSendValue;
public:
	CSendItem(){};
	CSendItem(const string &strRegId, const string &strDesAddr, const string & strSendValue)
	{
		m_strRegId = strRegId;
		m_strAddress = strDesAddr;
		m_strSendValue = strSendValue;
	}
	void GetContranctData(vector<unsigned char> &vContranct ) {
		CDataStream ds(SER_DISK, CLIENT_VERSION);
		ds << ParseHex(m_strRegId) << ParseHex(m_strSendValue);
		vContranct.insert(vContranct.end(), ds.begin(), ds.end());
	}
	//nFlag 0:ȡ����sendValues1��ֵ  1:ȡ����sendValues��ֵ
	//nIndex ȡֵ��Χ1~5����ʾ1~5���ͻ���
	static CSendItem GetRandomSendItem(int nFlag, int nIndex) {
		int randAddr = std::rand()%9;
		int randSendValue = std::rand()%10;
		map<string, string>::iterator iterAddr = mapDesAddress[nIndex-1].begin();
		do {
			iterAddr++;
		}while(randAddr-- && iterAddr != mapDesAddress[nIndex-1].end());
		return CSendItem(iterAddr->first, iterAddr->second, sendValues[nFlag][randSendValue]);
	}
	string GetRegID() {
		return m_strRegId;
	}

	string GetAddress() {
		return m_strAddress;
	}

	string GetSendValue() {
		return m_strSendValue;
	}


};
/**
 *������ͨ����
 * @param param
 * param[0]:Դ��ַ
 * param[1]:Ŀ�ĵ�ַ
 * param[2]:ת�˽��
 * param[3]:������
 * param[4]:��Ч�ڸ߶�
 */
void CreateNormalTx(vector<string> &param) {
	if(5 != param.size())
		return;
	param.insert(param.begin(), "sendtoaddress");
	param.insert(param.begin(), "rpctest");
//	char *argv[7] = { "rpctest", "createnormaltx", param.at(0), param.at(1), param.at(2), param.at(3), param.at(4)};
	char *argv[param.size()];
	int i=0;
	for(auto & item : param) {
		argv[i] = const_cast<char *>(param[i].c_str());
		++i;
	}
	CommandLineRPC(param.size(), argv);
}

/**
 * ������Լ����
 * @param param
 * param[0]:�ű�ע��ID
 * param[1]:�˻���ַ�б�,json�������ʽ
 * param[2]:��Լ����
 * param[3]:������
 * param[4]:��Ч�ڸ߶�
 */
void CreateContractTx(vector<string> &param) {
	if(5 != param.size())
		return;
	param.insert(param.begin(), "createcontracttx");
	param.insert(param.begin(), "rpctest");
	//char *argv[7] = {"rpctest", "createcontracttx", param.at(0), param.at(1), param.at(2), param.at(3), param.at(4)};
	char *argv[param.size()];
	int i=0;
	for(auto & item : param) {
		argv[i] = const_cast<char *>(param[i].c_str());
		++i;
	}
	CommandLineRPC(param.size(), argv);
}


/**
 * ����ע��ű�����
 * @param param
 * param[0]:ע��ű����˻���ַ
 * param[1]:ע��ű���ʶλ��0-��ʶ�ű����ݵ��ļ�·����1-��ע��ű�ID
 * param[2]:�ļ�·����ע��ű�ID
 * param[3]:������
 * param[4]:��Ч�ڸ߶�
 * param[5]:�ű����� �������ע��ű�,��ѡ��
 * param[6]:�ű���Ȩʱ�� ����ѡ��
 * param[7]:��Ȩ�ű�ÿ�δ��˻��пۼ�������� ����ѡ��
 * param[8]:��Ȩ�ű��ܹ���Ǯ������� ����ѡ��
 * param[9]:��Ȩ�ű�ÿ���Ǯ������� ����ѡ��
 * param[10]:�û��Զ�������
 *
 */
void CreateRegScriptTx(vector<string> &param) {
	if(5 > param.size())
		return;
	param.insert(param.begin(), "registerscripttx");
	param.insert(param.begin(), "rpctest");

	char *argv[param.size()];
	int i=0;
	for(auto & item : param) {
		argv[i] = const_cast<char *>(param[i].c_str());
		++i;
	}
	CommandLineRPC(param.size(), argv);
}

time_t string2time(const char * str,const char * formatStr)
{
  struct tm tm1;
  int year,mon,mday,hour,min,sec;
  if( -1 == sscanf(str,formatStr,&year,&mon,&mday,&hour,&min,&sec)) return -1;
  tm1.tm_year=year-1900;
  tm1.tm_mon=mon-1;
  tm1.tm_mday=mday;
  tm1.tm_hour=hour;
  tm1.tm_min=min;
  tm1.tm_sec=sec;
  return mktime(&tm1);
}
BOOST_FIXTURE_TEST_SUITE(auto_mining_test, CSendItem)
BOOST_AUTO_TEST_CASE(regscript) {
	//ע��ű�����
	vector<string> param;
	param.push_back("mvVp2PDRuG4JJh6UjkJFzXUC8K5JVbMFFA");
	param.push_back("0");
	param.push_back("D:\\cppwork\\vmsdk\\testUint\\Debug\\Exe\\test.bin");
	param.push_back("100000000");
	param.push_back("0");
	param.push_back("test");
	int64_t curTime = GetTime();
	char charTime[20] = {0};
	sprintf(charTime, "%ld", curTime);
	param.push_back(charTime);
	param.push_back("1000000000000");
	param.push_back("100000000000000");
	param.push_back("100000000000000");
	param.push_back("userdefine");
	CreateRegScriptTx(param);
}
BOOST_FIXTURE_TEST_CASE(test1, CSendItem)
{

//	time_t t1 = string2time("2014-12-01 17:30:00","%d-%d-%d %d:%d:%d");
	int64_t runTime = GetTime()+10*10*60;   //����ʮ����
	vector<string> param;
	while(GetTime()<runTime) {
		//�����ͻ���1->�ͻ���2����ͨ����
		CSendItem sendItem = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem = CSendItem::GetRandomSendItem(1,2);
		param.clear();
		param.push_back(sendItem.GetAddress());      	//Դ��ַ
		param.push_back(recItem.GetAddress());    	    //Ŀ�ĵ�ַ
		param.push_back(recItem.GetSendValue());	    //ת�˽��
		CreateNormalTx(param);                          //������ͨ����
		Sleep(1);

		//�����ͻ���1->�ͻ���2�ĺ�Լ����
		CSendItem sendItem1 = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem1 = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem2 = CSendItem::GetRandomSendItem(1,1);
		CSendItem recItem3 = CSendItem::GetRandomSendItem(1,1);
		param.clear();
		param.push_back("020000000100");                     //�ű�ID
		param.push_back("[\""+sendItem1.GetAddress()+"\"]"); //���׷����ַ
		vector<unsigned char> vContranct;
		vContranct.clear();
		vector<unsigned char> vTemp;
		vTemp.clear();
		recItem1.GetContranctData(vTemp);
		vContranct.insert(vContranct.end(), vTemp.begin(), vTemp.end());
		vTemp.clear();
		recItem2.GetContranctData(vTemp);
		vContranct.insert(vContranct.end(), vTemp.begin(), vTemp.end());
		recItem3.GetContranctData(vTemp);
		vContranct.insert(vContranct.end(), vTemp.begin(), vTemp.end());
		param.push_back(HexStr(vContranct));			//��Լ����
		param.push_back("100000000");					//������
		param.push_back("0");                           //��Ч�߶�
		CreateContractTx(param);                        //������Լ����
		Sleep(1);

	}
}
//BOOST_AUTO_TEST_CASE(test2)
//{
//
//}
//BOOST_AUTO_TEST_CASE(test3)
//{
//
//}
//
//BOOST_AUTO_TEST_CASE(test4)
//{
//
//}
//BOOST_AUTO_TEST_CASE(test5)
//{
//
//}

BOOST_AUTO_TEST_SUITE_END()
