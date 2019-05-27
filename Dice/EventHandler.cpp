#include "eventHandler.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>
#include <algorithm>
#include <atlstr.h>
#include <vector>

#include "APPINFO.h"
#include "RandomGenerator.h"
#include "RD.h"
#include "Smtp.h"
#include "CQEVE_ALL.h"
#include "InitList.h"
#include "GlobalVar.h"
#include "NameStorage.h"
#include "GetRule.h"
#include "DiceMsgSend.h"
#include "CustomMsg.h"
#include "NameGenerator.h"
#include "MsgFormat.h"
#include "DiceNetwork.h"
#include "eventHandler.h"
#include "RDConstant.h"
#include "direct.h"
#include "RandomReply.h"
#include "CardDeck.h"
#include <ctime>
#include <time.h>

#include <cmath>
#include <queue>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <functional>



using namespace std;
using namespace CQ;

unique_ptr<NameStorage> Name;
unique_ptr<LogStorage> Log;

int getNumber(const char *src)
{
	char chars;
	char *result;
	result = &chars;
	for (int i = 0; i < sizeof(src); i++)
	{
		if (src[i] >= 0 && src[i] <= 9)
		{
			result += src[i];
		}
	}
	return atoi(result);
}

vector<string> split(const string &str, const string &delim)
{
	vector<string> res;
	if ("" == str)
		return res;
	//�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����
	char *strs = new char[str.length() + 1]; //��Ҫ����
	strcpy(strs, str.c_str());

	char *d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while (p)
	{
		string s = p;	 //�ָ�õ����ַ���ת��Ϊstring����
		res.push_back(s); //����������
		p = strtok(NULL, d);
	}

	return res;
}

std::string changetoString(long long input)
{
	ostringstream os;
	os << input;
	string output;
	istringstream is(os.str());
	is >> output;
	return output;
}

std::string trim(string s)
{
	/*
	if( !s.empty() )
	{
		s.erase(0,s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
	}
	*/
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
	return s;
}

std::string strip(std::string origin)
{
	bool flag = true;
	while (flag)
	{
		flag = false;
		if (origin[0] == '!' || origin[0] == '.')
		{
			origin.erase(origin.begin());
			flag = true;
		}
		else if (origin.substr(0, 2) == "��" || origin.substr(0, 2) == "��")
		{
			origin.erase(origin.begin());
			origin.erase(origin.begin());
			flag = true;
		}
	}
	return origin;
}

map<long long, int> InfoTime;
int day = -1;
int InfoTimeRemain(long long a)
{
	if (InfoTime.count(a) != 0)
	{
		return InfoTime[a];
	}
	else
	{
		InfoTime[a] = 5;
		return InfoTime[a];
	}
}
void InfoTimeReduce(long long a)
{
	InfoTime[a] -= 1;
}
string InfoTimeUpdata()
{
	time_t rawtime;
	struct tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	if (day == -1)
	{
		day = ptminfo->tm_mday;
		for (map<long long, int>::iterator it = InfoTime.begin(); it != InfoTime.end(); it++)
		{
			InfoTime[it->first] = 5;
		}
	}
	else if (day != ptminfo->tm_mday)
	{
		day = ptminfo->tm_mday;
		for (map<long long, int>::iterator it = InfoTime.begin(); it != InfoTime.end(); it++)
		{
			InfoTime[it->first] = 5;
		}
	}
	return to_string(day) + ":" + to_string(ptminfo->tm_mday);
}

map<long long, int> EmailTime;
int emailTimeRemain(long long a)
{
	if (EmailTime.count(a) != 0)
	{
		return EmailTime[a];
	}
	else
	{
		EmailTime[a] = 5;
		return EmailTime[a];
	}
}
void emailTimeReduce(long long a)
{
	EmailTime[a] -= 1;
}
string emailTimeUpdata()
{
	time_t rawtime;
	struct tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	if (day == -1)
	{
		day = ptminfo->tm_mday;
		for (map<long long, int>::iterator it = EmailTime.begin(); it != EmailTime.end(); it++)
		{
			EmailTime[it->first] = 5;
		}
	}
	else if (day != ptminfo->tm_mday)
	{
		day = ptminfo->tm_mday;
		for (map<long long, int>::iterator it = EmailTime.begin(); it != EmailTime.end(); it++)
		{
			EmailTime[it->first] = 5;
		}
	}
	return to_string(day) + ":" + to_string(ptminfo->tm_mday);
}

vector<string> split(string &str, const char *c)
{
	char *cstr, *p;
	vector<string> res;
	cstr = new char[str.size() + 1];
	strcpy(cstr, str.c_str());
	p = strtok(cstr, c);
	while (p != NULL)
	{
		res.push_back(p);
		p = strtok(NULL, c);
	}
	return res;
}

bool IsNumber(const char *str)
{

	double aa;
	int nn = sscanf_s(str, "%lf", &aa);
	return nn != 0;
}

bool AllisNum(string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		int tmp = (int)str[i];
		if (tmp >= 48 && tmp <= 57)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

char *splitChar(string &str, const char *c)
{
	char *cstr, *p;
	cstr = new char[str.size() + 1];
	strcpy(cstr, str.c_str());
	p = strtok(cstr, c);
	int i = 0;
	return p;
}

int Checker(int checknum, int skillnum,int checktype)
{
	if (checktype == 1)
	{
		if (checknum <= 5 && checknum <= skillnum)return 1;
		else if (checknum <= skillnum / 5)return 2;
		else if (checknum <= skillnum / 2)return 3;
		else if (checknum <= skillnum)return 4;
		else if (checknum <= 95)return 5;
		else return 6;
	}
	else if (checktype == 2)
	{
		if (checknum == 1 && skillnum <= 50 && checknum <= skillnum)return 1;
		else if (checknum <= 5 && skillnum >= 50 && checknum <= skillnum)return 1;
		else if (checknum <= skillnum / 5)return 2;
		else if (checknum <= skillnum / 2)return 3;
		else if (checknum <= skillnum)return 4;
		else if (checknum <= 95 || (skillnum >= 50 && checknum != 100))return 5;
		else return 6;
	}
	else
	{
		if (checknum == 1)return 1;
		else if (checknum <= skillnum / 5)return 2;
		else if (checknum <= skillnum / 2)return 3;
		else if (checknum <= skillnum)return 4;
		else if (checknum <= 95 || (skillnum >= 50 && checknum != 100))return 5;
		else return 6;
	}
}
//�춨
string  ResultGet(int num)
{

	if (num == 1)return "��ɹ�";
	else if (num == 2)return "���޳ɹ�";
	else if (num == 3)return "���ѳɹ�";
	else if (num == 4)return "�ɹ�";
	else if (num == 5)return "ʧ��";
	else return "��ʧ��";
}
//���ݼ춨������ؼ���ֵ
std::string getName(long long QQ, long long GroupID = 0)
{
	// Ⱥ��������
	if (GroupID)
	{
		// Ⱥ���ǳ�
		if (!strip(Name->get(GroupID, QQ)).empty())
			return strip(Name->get(GroupID, QQ));
		// ˽��ȫ���ǳ�
		if (!strip(Name->get(0LL, QQ)).empty())
			return strip(Name->get(0LL, QQ));
		// Ⱥ��Ƭ-�������л᷵�ؿ��ַ���
		if (!getGroupMemberInfo(GroupID, QQ).GroupNick.empty())
			return strip(getGroupMemberInfo(GroupID, QQ).GroupNick);
		// �ǳ�
		return strip(getStrangerInfo(QQ).nick);
	}
	// ˽��
	// ˽��ȫ���ǳ�
	if (!strip(Name->get(0LL, QQ)).empty())
		return strip(Name->get(0LL, QQ));
	// �ǳ�
	return strip(getStrangerInfo(QQ).nick);
}

map<long long, int> DefaultDice;
const long long master = 2918762900;
map<long long, string> WelcomeMsg;
set<long long> DisabledGroup;
set<long long> DisabledDiscuss;
set<long long> DisabledJRRPGroup;
set<long long> DisabledJRRPDiscuss;
set<long long> DisabledHELPGroup;
set<long long> DisabledHELPDiscuss;
set<long long> DisabledOBGroup;
set<long long> DisabledOBDiscuss;
unique_ptr<Initlist> ilInitList;

struct SourceType
{
	SourceType(long long a, int b, long long c) : QQ(a), Type(b), GrouporDiscussID(c)
	{
	}
	SourceType(long long a, Dice::MsgType b, long long c) : QQ(a), Type(static_cast<int>(b)), GrouporDiscussID(c)
	{
	}
	long long QQ = 0;
	int Type = 0;
	long long GrouporDiscussID = 0;

	bool operator<(SourceType b) const
	{
		return this->QQ < b.QQ;
	}
};

struct ClueType
{
	ClueType(int a, long long b) : Type(a), GrouporDiscussID(b)
	{
	}
	ClueType(Dice::MsgType a, long long b) : Type(static_cast<int>(a)), GrouporDiscussID(b)
	{
	}
	int Type = 0;
	long long GrouporDiscussID = 0;

	bool operator<(ClueType a) const
	{
		return this->GrouporDiscussID < a.GrouporDiscussID;
	}
};

using PropType = map<string, int>;
using PropClueType = map<string, string>;
using TypeNumber = pair<long long, Dice::MsgType>;
using Listcon = map<string, PropType>;
using TeamSave =map<SourceType,string>;
map<SourceType, PropType> CharacterProp;
map<ClueType, PropClueType> ClueProp;
map<string, PropType>::iterator F2Fsk;
multimap<SourceType,long long>TeamSaver;
multimap<SourceType, Listcon>Clist;
map<SourceType, long long>duelT;
map<SourceType, TypeNumber>KpGroup;
map<TypeNumber, int[5]>GroupDuel;
map<SourceType, string>Usingname;
map<SourceType, int[9]> s_Count;
map<SourceType, int> Duellist;
multimap<long long, long long> ObserveGroup;
multimap<long long, long long> ObserveDiscuss;
char arr[80][80];
string strFileLoc;
SYSTEMTIME stNow = { 0 };
SYSTEMTIME stTmp = { 0 };
bool ClistFounder(SourceType d, string name)
{
	map<string, PropType>::iterator finder2;
	const auto range = Clist.equal_range(d);
	multimap<SourceType, Listcon>::iterator Cfinder1 = range.first;
	for (; Cfinder1 != range.second;)
	{
		finder2 = Cfinder1->second.begin();
		if (finder2->first == name)
		{
			F2Fsk = finder2;
			return 1;
		}
		if (++Cfinder1 == Clist.end())return 0;
	}
	return 0;
}
string DBgetter(int a)
{
	if (a <= 64)return "-2";
	else if (65 <= a&&a <= 84)return "-1";
	else if (85 <= a&&a <= 124)return"0";
	else if (125 <= a&&a <= 164)return"1d4";
	else 
	{
		string strRE = to_string((a - 164) / 80) + "d6";
		return strRE;
	}
}
string getDetail(SourceType a, string b)
{
	string strReturn;
	if (b == "default")
	{
		if (CharacterProp[a].count("����") && CharacterProp[a].count("����") && CharacterProp[a].count("��־") && CharacterProp[a].count("����") && CharacterProp[a].count("����"))
		{
			strReturn += "\nsan:" + to_string(CharacterProp[a]["����"]) + "/" + to_string(CharacterProp[a]["��־"]) + "\nDB:" + DBgetter(CharacterProp[a]["����"] + CharacterProp[a]["����"]) + "  hp:" + to_string(int(ceil((CharacterProp[a]["����"] + CharacterProp[a]["����"]) / 10)));
		}
	}
	else
	{
		bool checker = ClistFounder(a, b);
		if (checker && F2Fsk->second.count("����") && F2Fsk->second.count("����") && F2Fsk->second.count("��־") && F2Fsk->second.count("����") && F2Fsk->second.count("����"))
		{
			strReturn += "san:" + to_string(F2Fsk->second["����"]) + "/" + to_string(F2Fsk->second["��־"]) + "  \nDB:" + DBgetter(F2Fsk->second["����"] + F2Fsk->second["����"]) + "hp:" + to_string(int(ceil((F2Fsk->second["����"] + F2Fsk->second["����"]) / 10)));
		}
	}
	if (strReturn.empty()) return "���￨���ݲ��㣬�޷���ʾ����";
	else return strReturn;
}
//���׼�ʱ��
string getUname(SourceType a)
{
	if (Usingname.count(a))return Usingname[a];
	else
	{
		string b = getName(a.QQ) + "(default)";
		return b;
	}
}
void ConsoleTimer()
{
	while (Enabled)
	{
		AddMsgToQueue(Dice::DiceMsg("��ʼ��ʱ��10Сʱ�󽫶����ݽ��б���", 0LL, master, Dice::MsgType::Private));
		Sleep(36000000);
		GetLocalTime(&stNow);
			ofstream ofstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf", ios::out | ios::trunc);
			for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); ++it)
			{
				ofstreamDisabledGroup << *it << std::endl;
			}
			ofstreamDisabledGroup.close();

			ofstream ofstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf", ios::out | ios::trunc);
			for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); ++it)
			{
				ofstreamDisabledDiscuss << *it << std::endl;
			}
			ofstreamDisabledDiscuss.close();
			ofstream ofstreamDisabledJRRPGroup(strFileLoc + "DisabledJRRPGroup.RDconf", ios::out | ios::trunc);
			for (auto it = DisabledJRRPGroup.begin(); it != DisabledJRRPGroup.end(); ++it)
			{
				ofstreamDisabledJRRPGroup << *it << std::endl;
			}
			ofstreamDisabledJRRPGroup.close();

			ofstream ofstreamDisabledJRRPDiscuss(strFileLoc + "DisabledJRRPDiscuss.RDconf", ios::out | ios::trunc);
			for (auto it = DisabledJRRPDiscuss.begin(); it != DisabledJRRPDiscuss.end(); ++it)
			{
				ofstreamDisabledJRRPDiscuss << *it << std::endl;
			}
			ofstreamDisabledJRRPDiscuss.close();

			ofstream ofstreams_Count(strFileLoc + "s_Count.RDconf", ios::out | ios::trunc);
			for (auto it = s_Count.begin(); it != s_Count.end(); ++it)
			{
				ofstreams_Count << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
					<< it->second[1] << " "
					<< it->second[2] << " "
					<< it->second[3] << " "
					<< it->second[4] << " "
					<< it->second[5] << " "
					<< it->second[6] << " "
					<< it->second[7] << " "
					<< it->second[8] << std::endl;
			}
			ofstreams_Count.close();

			ofstream ofstreamDuellist(strFileLoc + "Duellist.RDconf", ios::out | ios::trunc);
			for (auto it = Duellist.begin(); it != Duellist.end(); ++it)
			{
				ofstreamDuellist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
					<< it->second << std::endl;
			}
			ofstreamDuellist.close();

			ofstream ofstreamClist(strFileLoc + "Clist.RDconf", ios::out | ios::trunc);
			for (auto it = Clist.begin(); it != Clist.end(); ++it)
			{
				for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
				{
					for (auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); ++it2)
					{
						ofstreamClist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
							<< it1->first << " " << it2->first << " " << it2->second << std::endl;
					}
				}
			}
			ofstreamClist.close();

			ofstream ofstreamUsingname(strFileLoc + "Usingname.RDconf", ios::out | ios::trunc);
			for (auto it = Usingname.begin(); it != Usingname.end(); ++it)
			{
				if(!it->second.empty())
				ofstreamUsingname << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " " << it->second << std::endl;
			}
			ofstreamUsingname.close();

			ofstream ofstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf", ios::in | ios::trunc);
			for (auto it = DisabledHELPGroup.begin(); it != DisabledHELPGroup.end(); ++it)
			{
				ofstreamDisabledHELPGroup << *it << std::endl;
			}
			ofstreamDisabledHELPGroup.close();

			ofstream ofstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf", ios::in | ios::trunc);
			for (auto it = DisabledHELPDiscuss.begin(); it != DisabledHELPDiscuss.end(); ++it)
			{
				ofstreamDisabledHELPDiscuss << *it << std::endl;
			}
			ofstreamDisabledHELPDiscuss.close();

			ofstream ofstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf", ios::out | ios::trunc);
			for (auto it = DisabledOBGroup.begin(); it != DisabledOBGroup.end(); ++it)
			{
				ofstreamDisabledOBGroup << *it << std::endl;
			}
			ofstreamDisabledOBGroup.close();

			ofstream ofstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf", ios::out | ios::trunc);
			for (auto it = DisabledOBDiscuss.begin(); it != DisabledOBDiscuss.end(); ++it)
			{
				ofstreamDisabledOBDiscuss << *it << std::endl;
			}
			ofstreamDisabledOBDiscuss.close();

			ofstream ofstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf", ios::out | ios::trunc);
			for (auto it = ObserveGroup.begin(); it != ObserveGroup.end(); ++it)
			{
				ofstreamObserveGroup << it->first << " " << it->second << std::endl;
			}
			ofstreamObserveGroup.close();

			ofstream ofstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf", ios::out | ios::trunc);
			for (auto it = ObserveDiscuss.begin(); it != ObserveDiscuss.end(); ++it)
			{
				ofstreamObserveDiscuss << it->first << " " << it->second << std::endl;
			}
			ofstreamObserveDiscuss.close();
			ofstream ofstreamCharacterProp(strFileLoc + "CharacterProp.RDconf", ios::out | ios::trunc);
			for (auto it = CharacterProp.begin(); it != CharacterProp.end(); ++it)
			{
				for (auto it1 = it->second.cbegin(); it1 != it->second.cend(); ++it1)
				{
					ofstreamCharacterProp << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
						<< it1->first << " " << it1->second << std::endl;
				}
			}
			ofstreamCharacterProp.close();
			ofstream ofstreamDefault(strFileLoc + "Default.RDconf", ios::out | ios::trunc);
			for (auto it = DefaultDice.begin(); it != DefaultDice.end(); ++it)
			{
				ofstreamDefault << it->first << " " << it->second << std::endl;
			}
			ofstreamDefault.close();

			ofstream ofstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf", ios::out | ios::trunc);
			for (auto it = WelcomeMsg.begin(); it != WelcomeMsg.end(); ++it)
			{
				while (it->second.find(' ') != string::npos)
					it->second.replace(it->second.find(' '), 1, "{space}");
				while (it->second.find('\t') != string::npos)
					it->second.replace(it->second.find('\t'), 1, "{tab}");
				while (it->second.find('\n') != string::npos)
					it->second.replace(it->second.find('\n'), 1, "{endl}");
				while (it->second.find('\r') != string::npos)
					it->second.replace(it->second.find('\r'), 1, "{enter}");
				ofstreamWelcomeMsg << it->first << " " << it->second << std::endl;
			}
			ofstreamWelcomeMsg.close();
			AddMsgToQueue(Dice::DiceMsg("�Զ�����ɹ�����", 0LL, master, Dice::MsgType::Private));
	}
}
namespace Dice
{
	void EventHandler::HandleEnableEvent()
	{
		//Wait until the thread terminates
		while (msgSendThreadRunning)
			Sleep(10);

		thread msgSendThread(SendMsg);
		msgSendThread.detach();
		thread threadConsoleTimer(ConsoleTimer);
		threadConsoleTimer.detach();
		strFileLoc = getAppDirectory();
		/*
				* ���ƴ洢-�������ȡ
				*/
		Name = make_unique<NameStorage>(strFileLoc + "Name.dicedb");
		Log = make_unique<LogStorage>(strFileLoc + "Log.dicedb");
		ifstream ifstreamCharacterProp(strFileLoc + "CharacterProp.RDconf");
		if (ifstreamCharacterProp)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value;
			string SkillName;
			while (ifstreamCharacterProp >> QQ >> Type >> GrouporDiscussID >> SkillName >> Value)
			{
				CharacterProp[SourceType(QQ, Type, GrouporDiscussID)][SkillName] = Value;
			}
		} 
		ifstreamCharacterProp.close();

		ifstream ifstreams_Count(strFileLoc + "s_Count.RDconf");
		if (ifstreams_Count)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value1, Value2, Value3, Value4, Value5, Value6, Value7, Value8;
			while (ifstreams_Count >> QQ >> Type >> GrouporDiscussID >> Value1 >> Value2 >> Value3 >> Value4 >> Value5 >> Value6 >> Value7 >> Value8)
			{
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][1] = Value1;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][2] = Value2;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][3] = Value3;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][4] = Value4;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][5] = Value5;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][6] = Value6;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][7] = Value7;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][8] = Value8;
			}
		}
		ifstreams_Count.close();

		ifstream ifstreamDuellist(strFileLoc + "Duellist.RDconf");
		if (ifstreamDuellist)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value;
			while (ifstreamDuellist >> QQ >> Type >> GrouporDiscussID >> Value)
			{
				Duellist[SourceType(QQ, Type, GrouporDiscussID)] = Value;
			}
		}
		ifstreamDuellist.close();

		ifstream ifstreamClist(strFileLoc + "Clist.RDconf");
		if (ifstreamClist)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value;
			string Skillname, StoCname;
			while (ifstreamClist >> QQ >> Type >> GrouporDiscussID >> StoCname >> Skillname >> Value)
			{
				PropType proptype1;
				Listcon listcon1;
				proptype1[Skillname] = Value;
				listcon1[StoCname] = proptype1;
				const auto range = Clist.equal_range(SourceType(QQ, Type, GrouporDiscussID));
				multimap<SourceType, Listcon>::iterator Cfinder1 = range.first;
				map<string, PropType>::iterator finder2;
				bool judge = 1;
				for (; Cfinder1 != range.second;)
				{
					finder2 = Cfinder1->second.begin();
					if (finder2->first == StoCname)
					{
						finder2->second[Skillname] = Value;
						judge = 0;
						break;
					}
					if (++Cfinder1 == Clist.end())break;
				}
				if (judge)Clist.insert(make_pair(SourceType(QQ, Type, GrouporDiscussID), listcon1));
				listcon1.clear(); proptype1.clear();
			}
		}
		ifstreamClist.close();

		ifstream ifstreamUsingname(strFileLoc + "Usingname.RDconf");
		if (ifstreamUsingname)
		{
			long long QQ, GrouporDiscussID;
			int Type;
			string usingname1;
			while (ifstreamUsingname >> QQ >> Type >> GrouporDiscussID >> usingname1)
			{
				Usingname[SourceType(QQ, Type, GrouporDiscussID)] = usingname1;
			}
		}
		ifstreamUsingname.close();

		ifstream ifstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf");
		if (ifstreamDisabledGroup)
		{
			long long Group;
			while (ifstreamDisabledGroup >> Group)
			{
				DisabledGroup.insert(Group);
			}
		}
		ifstreamDisabledGroup.close();
		ifstream ifstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf");
		if (ifstreamDisabledDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledDiscuss >> Discuss)
			{
				DisabledDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledDiscuss.close();
		ifstream ifstreamDisabledJRRPGroup(strFileLoc + "DisabledJRRPGroup.RDconf");
		if (ifstreamDisabledJRRPGroup)
		{
			long long Group;
			while (ifstreamDisabledJRRPGroup >> Group)
			{
				DisabledJRRPGroup.insert(Group);
			}
		}
		ifstreamDisabledJRRPGroup.close();
		ifstream ifstreamDisabledJRRPDiscuss(strFileLoc + "DisabledJRRPDiscuss.RDconf");
		if (ifstreamDisabledJRRPDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledJRRPDiscuss >> Discuss)
			{
				DisabledJRRPDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledJRRPDiscuss.close();
		ifstream ifstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf");
		if (ifstreamDisabledHELPGroup)
		{
			long long Group;
			while (ifstreamDisabledHELPGroup >> Group)
			{
				DisabledHELPGroup.insert(Group);
			}
		}
		ifstreamDisabledHELPGroup.close();
		ifstream ifstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf");
		if (ifstreamDisabledHELPDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledHELPDiscuss >> Discuss)
			{
				DisabledHELPDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledHELPDiscuss.close();
		ifstream ifstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf");
		if (ifstreamDisabledOBGroup)
		{
			long long Group;
			while (ifstreamDisabledOBGroup >> Group)
			{
				DisabledOBGroup.insert(Group);
			}
		}
		ifstreamDisabledOBGroup.close();
		ifstream ifstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf");
		if (ifstreamDisabledOBDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledOBDiscuss >> Discuss)
			{
				DisabledOBDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledOBDiscuss.close();
		ifstream ifstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf");
		if (ifstreamObserveGroup)
		{
			long long Group, QQ;
			while (ifstreamObserveGroup >> Group >> QQ)
			{
				ObserveGroup.insert(make_pair(Group, QQ));
			}
		}
		ifstreamObserveGroup.close();

		ifstream ifstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf");
		if (ifstreamObserveDiscuss)
		{
			long long Discuss, QQ;
			while (ifstreamObserveDiscuss >> Discuss >> QQ)
			{
				ObserveDiscuss.insert(make_pair(Discuss, QQ));
			}
		}
		ifstreamObserveDiscuss.close();
		ifstream ifstreamDefault(strFileLoc + "Default.RDconf");
		if (ifstreamDefault)
		{
			long long QQ;
			int DefVal;
			while (ifstreamDefault >> QQ >> DefVal)
			{
				DefaultDice[QQ] = DefVal;
			}
		}
		ifstreamDefault.close();
		ifstream ifstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf");
		if (ifstreamWelcomeMsg)
		{
			long long GroupID;
			string Msg;
			while (ifstreamWelcomeMsg >> GroupID >> Msg)
			{
				while (Msg.find("{space}") != string::npos)
					Msg.replace(Msg.find("{space}"), 7, " ");
				while (Msg.find("{tab}") != string::npos)
					Msg.replace(Msg.find("{tab}"), 5, "\t");
				while (Msg.find("{endl}") != string::npos)
					Msg.replace(Msg.find("{endl}"), 6, "\n");
				while (Msg.find("{enter}") != string::npos)
					Msg.replace(Msg.find("{enter}"), 7, "\r");
				WelcomeMsg[GroupID] = Msg;
			}
		}
		ifstreamWelcomeMsg.close();
		ilInitList = make_unique<Initlist>(strFileLoc + "INIT.DiceDB");
		ifstream ifstreamCustomMsg(strFileLoc + "CustomMsg.json");
		if (ifstreamCustomMsg)
		{
			ReadCustomMsg(ifstreamCustomMsg);
		}
		ifstreamCustomMsg.close();
	}
	void EventHandler::HandleMsgEvent(DiceMsg dice_msg, bool &block_msg)
	{
		string Cname = "default";
		if (Usingname.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) != 0)
		Cname = Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)];
		if (Cname.empty())
		{
			Cname = "default";
			dice_msg.Reply("����δ֪������ĵ�ǰ���￨����Ϊ�գ�����¼����ܻᵼ�������˵����ݶ�ʧ�����Խ�������￨���Ƹ�Ϊ\"default\"�뾡����ϵmaster���޸����ܵ�bug~~");
		}
		//���մ�ʩ
		init(dice_msg.msg);
		while (isspace(static_cast<unsigned char>(dice_msg.msg[0])))
			dice_msg.msg.erase(dice_msg.msg.begin());
		string strAt = "[CQ:at,qq=" + to_string(getLoginQQ()) + "]";
		if (dice_msg.msg.substr(0, 6) == "[CQ:at")
		{
			if (dice_msg.msg.substr(0, strAt.length()) != strAt)
			{
				return;
			}
			dice_msg.msg = dice_msg.msg.substr(strAt.length());
		}
		emailTimeUpdata();
		InfoTimeUpdata();
		init2(dice_msg.msg);
		const string strNickName = getName(dice_msg.qq_id, dice_msg.group_id);
		if (dice_msg.msg != ".end" && dice_msg.msg != ".log off")
		{
			Log->record(dice_msg.group_id, strNickName, dice_msg.msg);
		}
		if (dice_msg.msg[0] != '.')
			return;
		int intMsgCnt = 1;
		while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
			intMsgCnt++;
		block_msg = true;
		string strLowerMessage = dice_msg.msg;
		transform(strLowerMessage.begin(), strLowerMessage.end(), strLowerMessage.begin(), [](unsigned char c) { return tolower(c); });

		if (strLowerMessage.substr(intMsgCnt, 3) == "bot")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string Command;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				Command += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string QQNum = strLowerMessage.substr(intMsgCnt, dice_msg.msg.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					if (dice_msg.msg_type == Dice::MsgType::Group)
					{
						if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
						{
							if (DisabledGroup.count(dice_msg.group_id))
							{
								DisabledGroup.erase(dice_msg.group_id);
								dice_msg.Reply(GlobalMsg["strSuccessfullyEnabledNotice"]);
							}
							else
							{
								dice_msg.Reply(GlobalMsg["strAlreadyEnabledErr"]);
							}
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
						}
					}
					else if (dice_msg.msg_type == Dice::MsgType::Discuss)
					{
						if (DisabledDiscuss.count(dice_msg.group_id))
						{
							DisabledDiscuss.erase(dice_msg.group_id);
							dice_msg.Reply(GlobalMsg["strSuccessfullyEnabledNotice"]);
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strAlreadyEnabledErr"]);
						}
					}
				}
			}
			else if (Command == "off")
			{
				if (QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					if (dice_msg.msg_type == Dice::MsgType::Group)
					{
						if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
						{
							if (!DisabledGroup.count(dice_msg.group_id))
							{
								DisabledGroup.insert(dice_msg.group_id);
								dice_msg.Reply(GlobalMsg["strSuccessfullyDisabledNotice"]);
							}
							else
							{
								dice_msg.Reply(GlobalMsg["strAlreadyDisabledErr"]);
							}
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
						}
					}
					else if (dice_msg.msg_type == Dice::MsgType::Discuss)
					{
						if (!DisabledDiscuss.count(dice_msg.group_id))
						{
							DisabledDiscuss.insert(dice_msg.group_id);
							dice_msg.Reply(GlobalMsg["strSuccessfullyDisabledNotice"]);
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strAlreadyDisabledErr"]);
						}
					}
				}
			}
			else
			{
				if (QQNum.empty() || QQNum == to_string(getLoginQQ()) || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)))
				{
					dice_msg.Reply(Dice_Full_Ver);
				}
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 7) == "dismiss")
		{
			intMsgCnt += 7;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string QQNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				QQNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (QQNum.empty() || (QQNum.length() == 4 && QQNum == to_string(getLoginQQ() % 10000)) || QQNum == to_string(getLoginQQ()))
			{
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
					{
						setGroupLeave(dice_msg.group_id);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
					}
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					setDiscussLeave(dice_msg.group_id);
				}
				else
				{
					return;
				}
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 7) == "save"&& dice_msg.qq_id ==master)
		{
		CString path1 = strFileLoc.c_str();
		CString path2 = "/temfile";
		CString path = path1 + path2;
		if (!PathIsDirectory(path)) {
			::CreateDirectory(path, 0);
		}
		dice_msg.Reply("�����ֶ������У����Ժ�");
		ofstream ofstreamDisabledGroup(strFileLoc +"//temfile//" + "DisabledGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); ++it)
		{
			ofstreamDisabledGroup << *it << std::endl;
		}
		ofstreamDisabledGroup.close();

		ofstream ofstreamDisabledDiscuss(strFileLoc +"//temfile//" + "DisabledDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); ++it)
		{
			ofstreamDisabledDiscuss << *it << std::endl;
		}
		ofstreamDisabledDiscuss.close();
		ofstream ofstreamDisabledJRRPGroup(strFileLoc +"//temfile//" + "DisabledJRRPGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledJRRPGroup.begin(); it != DisabledJRRPGroup.end(); ++it)
		{
			ofstreamDisabledJRRPGroup << *it << std::endl;
		}
		ofstreamDisabledJRRPGroup.close();

		ofstream ofstreamDisabledJRRPDiscuss(strFileLoc +"//temfile//" + "DisabledJRRPDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledJRRPDiscuss.begin(); it != DisabledJRRPDiscuss.end(); ++it)
		{
			ofstreamDisabledJRRPDiscuss << *it << std::endl;
		}
		ofstreamDisabledJRRPDiscuss.close();

		ofstream ofstreams_Count(strFileLoc +"//temfile//" + "s_Count.RDconf", ios::out | ios::trunc);
		for (auto it = s_Count.begin(); it != s_Count.end(); ++it)
		{
			ofstreams_Count << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it->second[1] << " "
				<< it->second[2] << " "
				<< it->second[3] << " "
				<< it->second[4] << " "
				<< it->second[5] << " "
				<< it->second[6] << " "
				<< it->second[7] << " "
				<< it->second[8] << std::endl;
		}
		ofstreams_Count.close();

		ofstream ofstreamDuellist(strFileLoc +"//temfile//" + "Duellist.RDconf", ios::out | ios::trunc);
		for (auto it = Duellist.begin(); it != Duellist.end(); ++it)
		{
			ofstreamDuellist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it->second << std::endl;
		}
		ofstreamDuellist.close();

		ofstream ofstreamClist(strFileLoc +"//temfile//" + "Clist.RDconf", ios::out | ios::trunc);
		for (auto it = Clist.begin(); it != Clist.end(); ++it)
		{
			for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
			{
				for (auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); ++it2)
				{
					ofstreamClist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
						<< it1->first << " " << it2->first << " " << it2->second << std::endl;
				}
			}
		}
		ofstreamClist.close();

		ofstream ofstreamUsingname(strFileLoc +"//temfile//" + "Usingname.RDconf", ios::out | ios::trunc);
		for (auto it = Usingname.begin(); it != Usingname.end(); ++it)
		{
			if (!it->second.empty())
			ofstreamUsingname << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " " << it->second << std::endl;
		}
		ofstreamUsingname.close();

		ofstream ofstreamDisabledHELPGroup(strFileLoc +"//temfile//" + "DisabledHELPGroup.RDconf", ios::in | ios::trunc);
		for (auto it = DisabledHELPGroup.begin(); it != DisabledHELPGroup.end(); ++it)
		{
			ofstreamDisabledHELPGroup << *it << std::endl;
		}
		ofstreamDisabledHELPGroup.close();

		ofstream ofstreamDisabledHELPDiscuss(strFileLoc +"//temfile//" + "DisabledHELPDiscuss.RDconf", ios::in | ios::trunc);
		for (auto it = DisabledHELPDiscuss.begin(); it != DisabledHELPDiscuss.end(); ++it)
		{
			ofstreamDisabledHELPDiscuss << *it << std::endl;
		}
		ofstreamDisabledHELPDiscuss.close();

		ofstream ofstreamDisabledOBGroup(strFileLoc +"//temfile//" + "DisabledOBGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledOBGroup.begin(); it != DisabledOBGroup.end(); ++it)
		{
			ofstreamDisabledOBGroup << *it << std::endl;
		}
		ofstreamDisabledOBGroup.close();

		ofstream ofstreamDisabledOBDiscuss(strFileLoc +"//temfile//" + "DisabledOBDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledOBDiscuss.begin(); it != DisabledOBDiscuss.end(); ++it)
		{
			ofstreamDisabledOBDiscuss << *it << std::endl;
		}
		ofstreamDisabledOBDiscuss.close();

		ofstream ofstreamObserveGroup(strFileLoc +"//temfile//" + "ObserveGroup.RDconf", ios::out | ios::trunc);
		for (auto it = ObserveGroup.begin(); it != ObserveGroup.end(); ++it)
		{
			ofstreamObserveGroup << it->first << " " << it->second << std::endl;
		}
		ofstreamObserveGroup.close();

		ofstream ofstreamObserveDiscuss(strFileLoc +"//temfile//" + "ObserveDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = ObserveDiscuss.begin(); it != ObserveDiscuss.end(); ++it)
		{
			ofstreamObserveDiscuss << it->first << " " << it->second << std::endl;
		}
		ofstreamObserveDiscuss.close();
		ofstream ofstreamCharacterProp(strFileLoc +"//temfile//" + "CharacterProp.RDconf", ios::out | ios::trunc);
		for (auto it = CharacterProp.begin(); it != CharacterProp.end(); ++it)
		{
			for (auto it1 = it->second.cbegin(); it1 != it->second.cend(); ++it1)
			{
				ofstreamCharacterProp << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
					<< it1->first << " " << it1->second << std::endl;
			}
		}
		ofstreamCharacterProp.close();
		ofstream ofstreamDefault(strFileLoc +"//temfile//" + "Default.RDconf", ios::out | ios::trunc);
		for (auto it = DefaultDice.begin(); it != DefaultDice.end(); ++it)
		{
			ofstreamDefault << it->first << " " << it->second << std::endl;
		}
		ofstreamDefault.close();

		ofstream ofstreamWelcomeMsg(strFileLoc +"//temfile//" + "WelcomeMsg.RDconf", ios::out | ios::trunc);
		for (auto it = WelcomeMsg.begin(); it != WelcomeMsg.end(); ++it)
		{
			while (it->second.find(' ') != string::npos)
				it->second.replace(it->second.find(' '), 1, "{space}");
			while (it->second.find('\t') != string::npos)
				it->second.replace(it->second.find('\t'), 1, "{tab}");
			while (it->second.find('\n') != string::npos)
				it->second.replace(it->second.find('\n'), 1, "{endl}");
			while (it->second.find('\r') != string::npos)
				it->second.replace(it->second.find('\r'), 1, "{enter}");
			ofstreamWelcomeMsg << it->first << " " << it->second << std::endl;
		}
		ofstreamWelcomeMsg.close();
		dice_msg.Reply("�ֶ�����ɹ�����");
		}
		else if (strLowerMessage.substr(intMsgCnt, 7) == "load" && dice_msg.qq_id == master && dice_msg.msg_type == Dice::MsgType::Private)
		{
		dice_msg.Reply("���ڶ�ȡ�����У����Ժ�....");
		ifstream ifstreamCharacterProp(strFileLoc +"//temfile//" + "CharacterProp.RDconf");
		if (ifstreamCharacterProp)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value;
			string SkillName;
			while (ifstreamCharacterProp >> QQ >> Type >> GrouporDiscussID >> SkillName >> Value)
			{
				CharacterProp[SourceType(QQ, Type, GrouporDiscussID)][SkillName] = Value;
			}
		}
		ifstreamCharacterProp.close();

		ifstream ifstreams_Count(strFileLoc +"//temfile//" + "s_Count.RDconf");
		if (ifstreams_Count)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value1, Value2, Value3, Value4, Value5, Value6, Value7, Value8;
			while (ifstreams_Count >> QQ >> Type >> GrouporDiscussID >> Value1 >> Value2 >> Value3 >> Value4 >> Value5 >> Value6 >> Value7 >> Value8)
			{
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][1] = Value1;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][2] = Value2;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][3] = Value3;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][4] = Value4;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][5] = Value5;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][6] = Value6;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][7] = Value7;
				s_Count[SourceType(QQ, Type, GrouporDiscussID)][8] = Value8;
			}
		}
		ifstreams_Count.close();

		ifstream ifstreamDuellist(strFileLoc +"//temfile//" + "Duellist.RDconf");
		if (ifstreamDuellist)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value;
			while (ifstreamDuellist >> QQ >> Type >> GrouporDiscussID >> Value)
			{
				Duellist[SourceType(QQ, Type, GrouporDiscussID)] = Value;
			}
		}
		ifstreamDuellist.close();

		ifstream ifstreamClist(strFileLoc +"//temfile//" + "Clist.RDconf");
		if (ifstreamClist)
		{
			long long QQ, GrouporDiscussID;
			int Type, Value;
			string Skillname, StoCname;
			while (ifstreamClist >> QQ >> Type >> GrouporDiscussID >> StoCname >> Skillname >> Value)
			{
				PropType proptype1;
				Listcon listcon1;
				proptype1[Skillname] = Value;
				listcon1[StoCname] = proptype1;
				const auto range = Clist.equal_range(SourceType(QQ, Type, GrouporDiscussID));
				multimap<SourceType, Listcon>::iterator Cfinder1 = range.first;
				map<string, PropType>::iterator finder2;
				bool judge = 1;
				for (; Cfinder1 != range.second;)
				{
					finder2 = Cfinder1->second.begin();
					if (finder2->first == StoCname)
					{
						finder2->second[Skillname] = Value;
						judge = 0;
						break;
					}
					if (++Cfinder1 == Clist.end())break;
				}
				if (judge)Clist.insert(make_pair(SourceType(QQ, Type, GrouporDiscussID), listcon1));
				listcon1.clear(); proptype1.clear();
			}
		}
		ifstreamClist.close();

		ifstream ifstreamUsingname(strFileLoc +"//temfile//" + "Usingname.RDconf");
		if (ifstreamUsingname)
		{
			long long QQ, GrouporDiscussID;
			int Type;
			string usingname1;
			while (ifstreamUsingname >> QQ >> Type >> GrouporDiscussID >> usingname1)
			{
				Usingname[SourceType(QQ, Type, GrouporDiscussID)] = usingname1;
			}
		}
		ifstreamUsingname.close();

		ifstream ifstreamDisabledGroup(strFileLoc +"//temfile//" + "DisabledGroup.RDconf");
		if (ifstreamDisabledGroup)
		{
			long long Group;
			while (ifstreamDisabledGroup >> Group)
			{
				DisabledGroup.insert(Group);
			}
		}
		ifstreamDisabledGroup.close();
		ifstream ifstreamDisabledDiscuss(strFileLoc +"//temfile//" + "DisabledDiscuss.RDconf");
		if (ifstreamDisabledDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledDiscuss >> Discuss)
			{
				DisabledDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledDiscuss.close();
		ifstream ifstreamDisabledJRRPGroup(strFileLoc +"//temfile//" + "DisabledJRRPGroup.RDconf");
		if (ifstreamDisabledJRRPGroup)
		{
			long long Group;
			while (ifstreamDisabledJRRPGroup >> Group)
			{
				DisabledJRRPGroup.insert(Group);
			}
		}
		ifstreamDisabledJRRPGroup.close();
		ifstream ifstreamDisabledJRRPDiscuss(strFileLoc +"//temfile//" + "DisabledJRRPDiscuss.RDconf");
		if (ifstreamDisabledJRRPDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledJRRPDiscuss >> Discuss)
			{
				DisabledJRRPDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledJRRPDiscuss.close();
		ifstream ifstreamDisabledHELPGroup(strFileLoc +"//temfile//" + "DisabledHELPGroup.RDconf");
		if (ifstreamDisabledHELPGroup)
		{
			long long Group;
			while (ifstreamDisabledHELPGroup >> Group)
			{
				DisabledHELPGroup.insert(Group);
			}
		}
		ifstreamDisabledHELPGroup.close();
		ifstream ifstreamDisabledHELPDiscuss(strFileLoc +"//temfile//" + "DisabledHELPDiscuss.RDconf");
		if (ifstreamDisabledHELPDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledHELPDiscuss >> Discuss)
			{
				DisabledHELPDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledHELPDiscuss.close();
		ifstream ifstreamDisabledOBGroup(strFileLoc +"//temfile//" + "DisabledOBGroup.RDconf");
		if (ifstreamDisabledOBGroup)
		{
			long long Group;
			while (ifstreamDisabledOBGroup >> Group)
			{
				DisabledOBGroup.insert(Group);
			}
		}
		ifstreamDisabledOBGroup.close();
		ifstream ifstreamDisabledOBDiscuss(strFileLoc +"//temfile//" + "DisabledOBDiscuss.RDconf");
		if (ifstreamDisabledOBDiscuss)
		{
			long long Discuss;
			while (ifstreamDisabledOBDiscuss >> Discuss)
			{
				DisabledOBDiscuss.insert(Discuss);
			}
		}
		ifstreamDisabledOBDiscuss.close();
		ifstream ifstreamObserveGroup(strFileLoc +"//temfile//" + "ObserveGroup.RDconf");
		if (ifstreamObserveGroup)
		{
			long long Group, QQ;
			while (ifstreamObserveGroup >> Group >> QQ)
			{
				ObserveGroup.insert(make_pair(Group, QQ));
			}
		}
		ifstreamObserveGroup.close();

		ifstream ifstreamObserveDiscuss(strFileLoc +"//temfile//" + "ObserveDiscuss.RDconf");
		if (ifstreamObserveDiscuss)
		{
			long long Discuss, QQ;
			while (ifstreamObserveDiscuss >> Discuss >> QQ)
			{
				ObserveDiscuss.insert(make_pair(Discuss, QQ));
			}
		}
		ifstreamObserveDiscuss.close();
		ifstream ifstreamDefault(strFileLoc +"//temfile//" + "Default.RDconf");
		if (ifstreamDefault)
		{
			long long QQ;
			int DefVal;
			while (ifstreamDefault >> QQ >> DefVal)
			{
				DefaultDice[QQ] = DefVal;
			}
		}
		ifstreamDefault.close();
		ifstream ifstreamWelcomeMsg(strFileLoc +"//temfile//" + "WelcomeMsg.RDconf");
		if (ifstreamWelcomeMsg)
		{
			long long GroupID;
			string Msg;
			while (ifstreamWelcomeMsg >> GroupID >> Msg)
			{
				while (Msg.find("{space}") != string::npos)
					Msg.replace(Msg.find("{space}"), 7, " ");
				while (Msg.find("{tab}") != string::npos)
					Msg.replace(Msg.find("{tab}"), 5, "\t");
				while (Msg.find("{endl}") != string::npos)
					Msg.replace(Msg.find("{endl}"), 6, "\n");
				while (Msg.find("{enter}") != string::npos)
					Msg.replace(Msg.find("{enter}"), 7, "\r");
				WelcomeMsg[GroupID] = Msg;
			}
		}
		ifstreamWelcomeMsg.close();
		ilInitList = make_unique<Initlist>(strFileLoc +"//temfile//" + "INIT.DiceDB");
		ifstream ifstreamCustomMsg(strFileLoc +"//temfile//" + "CustomMsg.json");
		if (ifstreamCustomMsg)
		{
			ReadCustomMsg(ifstreamCustomMsg);
		}
		ifstreamCustomMsg.close();
		dice_msg.Reply("��ȡ�ɹ����Ѿ�������ʱ���������");
		}
		else if ((dice_msg.msg_type == Dice::MsgType::Group && DisabledGroup.count(dice_msg.group_id)) || (dice_msg.msg_type == Dice::MsgType::Discuss && DisabledDiscuss.count(dice_msg.group_id)))
			return;
		else if (strLowerMessage.substr(intMsgCnt, 4) == "help")
		{
			intMsgCnt += 4;
			while (strLowerMessage[intMsgCnt] == ' ')
				intMsgCnt++;
			const string strAction = strLowerMessage.substr(intMsgCnt);
			if (strAction == "on")
			{
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
					{
						if (DisabledHELPGroup.count(dice_msg.group_id))
						{
							DisabledHELPGroup.erase(dice_msg.group_id);
							dice_msg.Reply(GlobalMsg["strHelpCommandSuccessfullyEnabledNotice"]);
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strHelpCommandAlreadyEnabledErr"]);
						}
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
					}
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					if (DisabledHELPDiscuss.count(dice_msg.group_id))
					{
						DisabledHELPDiscuss.erase(dice_msg.group_id);
						dice_msg.Reply(GlobalMsg["strHelpCommandSuccessfullyEnabledNotice"]);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strHelpCommandAlreadyEnabledErr"]);
					}
				}
				return;
			}
			if (strAction == "off")
			{
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
					{
						if (!DisabledHELPGroup.count(dice_msg.group_id))
						{
							DisabledHELPGroup.insert(dice_msg.group_id);
							dice_msg.Reply(GlobalMsg["strHelpCommandSuccessfullyDisabledNotice"]);
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strHelpCommandAlreadyDisabledErr"]);
						}
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
					}
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					if (!DisabledHELPDiscuss.count(dice_msg.group_id))
					{
						DisabledHELPDiscuss.insert(dice_msg.group_id);
						dice_msg.Reply(GlobalMsg["strHelpCommandSuccessfullyDisabledNotice"]);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strHelpCommandAlreadyDisabledErr"]);
					}
				}
				return;
			}
			if ((dice_msg.msg_type == Dice::MsgType::Group && DisabledHELPGroup.count(dice_msg.group_id)) || (dice_msg.msg_type == Dice::MsgType::Group && DisabledHELPDiscuss.count(dice_msg.group_id)))
			{
				dice_msg.Reply(GlobalMsg["strHelpCommandDisabledErr"]);
				return;
			}
			dice_msg.Reply(GlobalMsg["strHelpMsg"]);
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "hiy")
	{
	intMsgCnt += 3;
	string strReply;
	if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1] == NULL)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1] = 0;
	if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2] == NULL)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2] = 0;
	if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3] == NULL)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3] = 0;
	if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] == NULL)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] = 0;
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
	if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
	{
		s_Count.erase(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
		strReply = "�ɹ������ʷ��¼��";
		dice_msg.Reply(strReply);
	}
	else if (intMsgCnt == strLowerMessage.length())
	{
		int Re_number;
		if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3] != 0)
			Re_number = s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] / s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3];
		else
			Re_number = 0;
		strReply += "���ۼ��ӳ�\n" + to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]) + "����ɹ�," +
			to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]) + "����ʧ��\n" + "�ۼ�ʹ�ûݻ�����"
			+ to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3]) + "��\n" + "ƽ����ֵΪ��"
			+ to_string(Re_number) + "\n����Ϊֹ���Ҿ�����ʤ���ǡ���\nʤ" +
			to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][5]) + "��" +
			to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][6]) + "ƽ" +
			to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][7]) + "\n����Ϊֹ�õ�������߷���" +
			to_string(s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][8]) + "\n������Ŷ���ҿ�û�й�Ǧ";
		dice_msg.Reply(strReply);
	}
	}
		else if (strLowerMessage.substr(intMsgCnt, 7) == "welcome")
		{
			if (dice_msg.msg_type != Dice::MsgType::Group)
				return;
			intMsgCnt += 7;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
			{
				string strWelcomeMsg = dice_msg.msg.substr(intMsgCnt);
				if (strWelcomeMsg.empty())
				{
					if (WelcomeMsg.count(dice_msg.group_id))
					{
						WelcomeMsg.erase(dice_msg.group_id);
						dice_msg.Reply(GlobalMsg["strWelcomeMsgClearedNotice"]);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strWelcomeMsgIsEmptyErr"]);
					}
				}
				else
				{
					WelcomeMsg[dice_msg.group_id] = strWelcomeMsg;
					dice_msg.Reply(GlobalMsg["strWelcomeMsgUpdatedNotice"]);
				}
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "duel")
	{
	intMsgCnt += 4;
	if (strLowerMessage.substr(intMsgCnt, 4) == "list")
	{
		string Reply = "��ǰ�Ծ����а�\n1.�ݻݡ���100��";
		map<SourceType, int> temlist = Duellist;
		map<SourceType, int>::iterator Temmin1 = temlist.begin();
		const int clcy = temlist.size();
		if (temlist.empty() != 1)
		{
			for (unsigned int i = 0; i < clcy; i++)
			{
				for (auto each = temlist.begin(); each != temlist.end(); each++)
				{
					if (each->second > Temmin1->second)Temmin1 = each;
				}
				Reply += "\n" + to_string(i + 2) + "." + getName(Temmin1->first.QQ, Temmin1->first.GrouporDiscussID) + "����" + to_string(Temmin1->second) + "��";
				temlist.erase(Temmin1);
				Temmin1 = temlist.begin();
			}
		}
		dice_msg.Reply(Reply);
		return;
	}
	if (duelT.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) == 0)duelT[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = time(NULL);
	else if (time(NULL) - duelT[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] < 72000)
	{
		dice_msg.Reply("һ��ֻ�ܾ���һ��Ŷ����������Ҫ�ǲ������Ļ�����Ҳ����ֻ�ܾ���һ��\nʣ��" +
			to_string
			((72000 + duelT[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] - time(NULL)) / 3600)
			+ "ʱ" +
			to_string(((72000 + duelT[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] - time(NULL)) % 3600) / 60)
			+ "��"
		);
		return;
	}
	else duelT[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = time(NULL);
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	string strReply = "������" + strNickName + "�����ҷ�����������������Ҿͽ�����\n";
	int random[10] = { 0 };
	for (int i = 0; i <= 9; i++)	random[i] = RandomGenerator::Randint(1, 100);
	for (int i = 0; i <= 4; i++)
	{
		int bunus = RandomGenerator::Randint(1, 100);
		if (bunus > random[i])random[i] = bunus;
	}
	int totalscore1 = random[0] + random[1] + random[2] + random[3] + random[4];
	int totalscore2 = random[5] + random[6] + random[7] + random[8] + random[9];
	int score;
	if ((totalscore2 - totalscore1) / 10 != 0) score = (totalscore2 - totalscore1) / 10;
	else if (totalscore2 > totalscore1)score = 1;
	else if (totalscore2 < totalscore1)score = -1;
	else if (totalscore2 == totalscore1)score = 0;


	strReply += "ʤ���Ľ���ǡ���\n�ݻ�\nsum:" + to_string(totalscore1) + "\n" + to_string(random[0]) + "��" + to_string(random[1]) + "��" + to_string(random[2]) + "��" + to_string(random[3]) + "��" +
		to_string(random[4]) + "\n" + strNickName + "\nsum:" + to_string(totalscore2) + "\n" + to_string(random[5]) + "��" + to_string(random[6]) + "��" + to_string(random[7]) + "��" + to_string(random[8])
		+ "��" + to_string(random[9]) + "\n��������\n�ǣ�" + to_string(totalscore2 - totalscore1) + "/10=" + to_string(score) + "��";
	if (score > 0)			s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][5]++;
	else if (score < 0)		s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][6]++;
	else if (score == 0)	s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][7]++;
	if (score > s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][8])s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][8] = score;
	map<SourceType, int>::iterator Temmin;
	bool found = 0;
	Temmin = Duellist.begin();
	for (auto each = Duellist.begin(); each != Duellist.end(); each++)
	{
		if (each->second < Temmin->second)Temmin = each;
	}
	if (Temmin->second < score)found = 1;
	if (found && Duellist.size() >= 10 && (Duellist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) == 0 || Duellist[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] < score))
	{
		Temmin = Duellist.erase(Temmin);
		Duellist[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = score;
	}
	else if (Duellist.size() <= 10 && (Duellist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) == 0 || Duellist[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] < score))
	{
		Duellist.insert(pair<SourceType, int>(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id), score));
	}
	map<std::string, std::vector<std::string>>::const_iterator cit = RandomReply::mRanDeck.find("Duelresult");
	strReply += cit->second[score + 49];
	dice_msg.Reply(strReply);
	return;
	}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "st")
	{
	intMsgCnt += 2;
	string strCname;
	int begin = strLowerMessage.find(" ", intMsgCnt);//ȥ���ո�
	while (begin != -1)
	{
		strLowerMessage.replace(begin, 1, "");
		begin = strLowerMessage.find(" ");
	}//ȥ���ո�
	if (strLowerMessage.find('+') != -1)
	{
		strCname = Cname;
		Cname.clear();
		for (intMsgCnt; intMsgCnt != strLowerMessage.find('+'); intMsgCnt++)
		{
			Cname += strLowerMessage[intMsgCnt];
		}
		intMsgCnt += 1;
		if (Cname.empty() || Cname.length() >= 10)
		{
			dice_msg.Reply("��������￨����Ϊ�ջ��߹���������ˣ���");
			return;
		}
		Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = Cname;
	}
	//cname��ֵ
	bool CnameBl = 0;
	multimap<SourceType, Listcon>::iterator finder1;
	map<string, PropType>::iterator finder2;
	if (Cname != "default" && Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) != 0)
	{
		const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
		multimap<SourceType, Listcon>::iterator Cfinder1 = range.first;//����Ƿ����Ѿ����ڵ�Cname����ޣ�����һ�����һ�ȡ��Ӧλ�õ�����
		for (; Cfinder1 != range.second;)
		{
			finder2 = Cfinder1->second.begin();
			if (finder2->first == Cname)
			{
				F2Fsk = finder2;
				CnameBl = 1;
				break;
			}
			if (++Cfinder1 == Clist.end())break;
		}
	}
	if (intMsgCnt == strLowerMessage.length())
	{
		dice_msg.Reply(GlobalMsg["strStErr"]);
		Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = strCname;
		return;
	}
	if (strLowerMessage.substr(intMsgCnt, 3) == "clr" && Cname == "default")
	{
		intMsgCnt += 3;
		if (CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)))
		{
			CharacterProp.erase(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
		}
		dice_msg.Reply(GlobalMsg["strPropCleared"]);
		return;
	}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
	{
		intMsgCnt += 3;
		if (Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) != 0)
		{
			const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
			finder1 = range.first;
			for (; finder1 != range.second;)
			{
				finder2 = finder1->second.begin();
				if (finder2->first == Cname)
				{
					finder1 = Clist.erase(finder1);
					Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = "default";
					dice_msg.Reply(GlobalMsg["strPropCleared"]);
					return;
				}
				if (++finder1 == Clist.end() || Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) == 0)break;
			}
		}
		dice_msg.Reply(GlobalMsg["strPropClearErr"]);
		return;
	}
	if (strLowerMessage.substr(intMsgCnt, 3) == "del" && Cname == "default")
	{
		intMsgCnt += 3;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string strSkillName;
		while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
			intMsgCnt] == '|'))
		{
			strSkillName += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			if (CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].count(strSkillName))
			{
				CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].erase(strSkillName);
				dice_msg.Reply(GlobalMsg["strPropDeleted"]);
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPropNotFound"]);
			}
			return;
	}
	else if (strLowerMessage.substr(intMsgCnt, 3) == "del")
	{
		intMsgCnt += 3;
		string strSkillName;
		while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
			intMsgCnt] == '|'))
		{
			strSkillName += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			if (Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CnameBl &&
				F2Fsk->second.count(strSkillName))
			{
				F2Fsk->second.erase(strSkillName);
				dice_msg.Reply(GlobalMsg["strPropDeleted"]);
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPropNotFound"]);
			}
			return;
	}
	if (strLowerMessage[intMsgCnt] == 'q' && Cname == "default")
	{

		intMsgCnt += 1;
		string strReply = "st";
		SourceType character = SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id);
		if (CharacterProp.count(character))
		{
			for (auto each : CharacterProp[character])
				strReply += each.first + to_string(each.second);
			dice_msg.Reply(strReply);
		}
		else
		{
			dice_msg.Reply(GlobalMsg["strPropNotFound"]);
		}
		return;
	}
	else if (strLowerMessage[intMsgCnt] == 'q')
	{
		intMsgCnt += 1;
		string strReply;
		SourceType character = SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id);
		if (CnameBl)
		{
			strReply += "cst" + Cname + '+';
			for (auto each : finder2->second)
				strReply += each.first + to_string(each.second);
			dice_msg.Reply(strReply);
		}
		else
		{
			dice_msg.Reply(format(GlobalMsg["strCPropNotFound"], { Cname }));
		}
		return;
	}
	if (strLowerMessage.substr(intMsgCnt, 4) == "show" && Cname == "default")
	{
		intMsgCnt += 4;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string strSkillName;
		while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
			intMsgCnt] == '|'))
		{
			strSkillName += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			if (CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].count(strSkillName))
			{
				dice_msg.Reply(format(GlobalMsg["strProp"],
					{
						strNickName, strSkillName,to_string(CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName])
					}));
			}
			else if (SkillDefaultVal.count(strSkillName))
			{
				dice_msg.Reply(format(GlobalMsg["strProp"], { strNickName, strSkillName, to_string(SkillDefaultVal[strSkillName]) }));
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPropNotFound"]);
			}
			return;
	}
	else if (strLowerMessage.substr(intMsgCnt, 4) == "show")
	{
		intMsgCnt += 4;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		string strSkillName;
		while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[
			intMsgCnt] == '|'))
		{
			strSkillName += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			if (CnameBl && F2Fsk->second.count(strSkillName))
			{
				dice_msg.Reply(format(GlobalMsg["strProp"],
					{
						strNickName, strSkillName,to_string(F2Fsk->second[strSkillName])
					}));
			}
			else if (SkillDefaultVal.count(strSkillName))
			{
				dice_msg.Reply(format(GlobalMsg["strProp"], { strNickName, strSkillName, to_string(SkillDefaultVal[strSkillName]) }));
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPropNotFound"]);
			}
			return;
	}
	if (Cname == "default")
	{
		bool boolError = false;
		while (intMsgCnt != strLowerMessage.length())
		{
			string strSkillName;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
				isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt]
				!= ':')
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt
			] == ':')intMsgCnt++;
			string strSkillVal;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strSkillVal += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strSkillName.empty() || strSkillVal.empty() || strSkillVal.length() > 3)
			{
				boolError = true;
				break;
			}
			if (SkillDefaultVal[strSkillName] != stoi(strSkillVal))
			CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName] = stoi(strSkillVal);
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '|')intMsgCnt++;
		}
		if (boolError)
		{
			dice_msg.Reply(GlobalMsg["strPropErr"]);
		}
		else
		{
			dice_msg.Reply(GlobalMsg["strSetPropSuccess"]);
		}
	}
	else
	{
		bool SuccIn = 0;
		if (Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) >= 6 && CnameBl != 1)
		{
			dice_msg.Reply(GlobalMsg["strCnameErr"]);
			Cname = strCname;
			Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = strCname;
			return;
		}
		if (CnameBl == 0)
		{
			PropType Cconfig;
			Listcon Clison;
			Clison[Cname] = Cconfig;
			Clist.insert(make_pair(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id), Clison));
			const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
			finder1 = range.first;//ȷ���Ѿ����룬������Error
			for (; finder1 != range.second;)
			{
				finder2 = finder1->second.begin();
				if (finder2->first == Cname)
				{
					F2Fsk = finder2;
					SuccIn = 1;
					break;
				}
				if (++finder1 == Clist.end())break;
			}
			if (SuccIn)dice_msg.Reply(GlobalMsg["strInsert"]);
			else
			{
				dice_msg.Reply(GlobalMsg["strCerror"]);
				return;
			}
		}
		while (intMsgCnt != strLowerMessage.length())
		{
			string strSkillName;
			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
				isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt]
				!= ':')
			{
				strSkillName += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
			while (strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] == ':')intMsgCnt++;
			string strSkillVal;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strSkillVal += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strSkillName.empty() || strSkillVal.empty() || strSkillVal.length() > 3)
			{
				dice_msg.Reply(GlobalMsg["strPropErr"]);
				return;
			}
			if (SkillDefaultVal[strSkillName] != stoi(strSkillVal))
			F2Fsk->second[strSkillName] = stoi(strSkillVal);
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '|')intMsgCnt++;
		}
		dice_msg.Reply(GlobalMsg["strSetPropSuccess"]);
		return;
	}
	}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "cha")
	{
	string strReply;
	string Dname = Cname;
	intMsgCnt += 3;
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	multimap<SourceType, Listcon>::iterator finder1;
	map<string, PropType>::iterator finder2;
	finder1 = Clist.find(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
	if (strLowerMessage.substr(intMsgCnt, 2) == "me")
	{
		strReply += "�㵱ǰʹ�õ����￨��:\n" + Cname;
		bool found = 0;
		if (Cname != "default")found = ClistFounder(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id), Cname);
		if (found)
		{
			strReply += "\n���￨״̬\n"+getDetail(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id), Cname);
		}
		if (Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) != 0)
		{
			strReply += "\n��ĵ���Ա�С�����";
			const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
			for (auto it = range.first; it != range.second; ++it)
			{
				finder2 = it->second.begin();
				strReply += "\n" + finder2->first;
			}
			dice_msg.Reply(strReply);
			return;
		}
		else if (Clist.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) == 0)
		{
			strReply += "\n������￨�б��ǿ��գ�";
			dice_msg.Reply(strReply);
		}
	}
	if (strLowerMessage.substr(intMsgCnt, 3) == "era")
	{
		intMsgCnt += 3;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		if (intMsgCnt == strLowerMessage.length())
		{
			Clist.erase(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
			dice_msg.Reply(GlobalMsg["strClistCleared"]);
			Cname = "default";
			Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = Cname;
			return;
		}
		else
		{
			Dname.erase();
			while (intMsgCnt != strLowerMessage.length())
			{
				Dname += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
			multimap<SourceType, Listcon>::iterator Cfinder1 = range.first;
			for (; Cfinder1 != range.second;)
			{
				finder2 = Cfinder1->second.begin();
				if (finder2->first == Dname)
				{
					Cfinder1 = Clist.erase(Cfinder1);
					if (Cname == Dname)
					{
						Cname = "default";
						Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = Cname;
					}
					break;
				}
				if (++Cfinder1 == Clist.end())break;
			}
			dice_msg.Reply(format(GlobalMsg["strCharErased"], { Dname }));
			return;
		}
	}
	if (strLowerMessage.substr(intMsgCnt, 3) == "set")
	{
		intMsgCnt += 3;
		string Temname;
		bool searchRes = 0;
		while (intMsgCnt != strLowerMessage.length())
		{
			Temname += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
		if (Temname.empty())Temname = "default";
		const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
		multimap<SourceType, Listcon>::iterator Serfinder = range.first;
		for (; Serfinder != range.second;)
		{
			finder2 = Serfinder->second.begin();
			if (finder2->first == Temname)
			{
				searchRes = 1;
				break;
			}
			if (++Serfinder == Clist.end())break;
		}
		if (searchRes || Temname == "default")
		{
			Usingname[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = Temname;
			dice_msg.Reply(format(GlobalMsg["strCharsetted"], { Temname }));
		}

		else dice_msg.Reply(format(GlobalMsg["strChanotFind"], { Temname }));
	}
	}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "kp")
	{
			if (dice_msg.msg_type != Dice::MsgType::Private)
			{
				intMsgCnt += 2;
				string command;
				while (intMsgCnt != strLowerMessage.length())
				{
					command += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				if (command == "show" && KpGroup.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)))
				{
					dice_msg.Reply("�㵱ǰ��KPȺ/������Ϊ��\n" + to_string(KpGroup[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].first));
					return;
				}
				else if (command == "show")
				{
					dice_msg.Reply("�㵱ǰû��KPȺ/������");
					return;
				}
				else if (command == "era")
				{
					KpGroup.erase(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
					dice_msg.Reply("�Ѿ��ɹ��Ƴ���ĵ�ǰKPȺ/������");
					return;
				}
				else
				{
					KpGroup[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)] = {dice_msg.group_id, dice_msg.msg_type};
					dice_msg.Reply(GlobalMsg["strKPsetted"]);
				}
			}
	return;

	}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "draw")
	{
	intMsgCnt += 4;
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;

	string strDeckName;
	while (!isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
	{
		strDeckName += strLowerMessage[intMsgCnt];
		intMsgCnt++;
	}
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	string strCardNum;
	while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
	{
		strCardNum += strLowerMessage[intMsgCnt];
		intMsgCnt++;
	}
	auto intCardNum = strCardNum.empty() ? 1 : stoi(strCardNum);
	if (intCardNum == 0)
	{
		dice_msg.Reply(GlobalMsg["strNumCannotBeZero"]);
		return;
	}
	int intFoundRes = CardDeck::findDeck(strDeckName);
	string strReply;
	if (intFoundRes == 0) {
		strReply = "��˵" + strDeckName + "?" + GlobalMsg["strDeckNotFound"];
		dice_msg.Reply(strReply);
		return;
	}
	vector<string> TempDeck(CardDeck::mPublicDeck[strDeckName]);
	strReply = format(GlobalMsg["strDrawCard"], { strNickName , CardDeck::drawCard(TempDeck) });
	while (--intCardNum && TempDeck.size()) {
		strReply += "\n" + CardDeck::drawCard(TempDeck);
		if (strReply.length() > 1000) {
			dice_msg.Reply(strReply);
			strReply.clear();
		}
	}
	dice_msg.Reply(strReply);
	if (intCardNum) {
		dice_msg.Reply(GlobalMsg["strDeckEmpty"]);
		return;
	}
	}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "clue")
		{
			intMsgCnt += 4;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (intMsgCnt == strLowerMessage.length())
			{
				dice_msg.Reply(GlobalMsg["strClueErr"]);
				return;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
			{
				if (ClueProp.count(ClueType(dice_msg.msg_type, dice_msg.group_id)))
				{
					ClueProp.erase(ClueType(dice_msg.msg_type, dice_msg.group_id));
				}
				dice_msg.Reply(GlobalMsg["strClueCleared"]);
				return;
			}
			if (strLowerMessage.substr(intMsgCnt, 4) == "show")
			{
				intMsgCnt += 4;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
				string ClueInfo = "Clue";
				while (intMsgCnt != strLowerMessage.length() && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !(strLowerMessage[intMsgCnt] == '|'))
				{
					ClueInfo = "Clue";
					intMsgCnt++;
				}
				if (ClueProp.count(ClueType(dice_msg.msg_type, dice_msg.group_id)) && ClueProp[ClueType(
					dice_msg.msg_type, dice_msg.group_id)]
					.count(ClueInfo))
				{
					dice_msg.Reply(GlobalMsg["strClue"] +
						ClueProp[ClueType(dice_msg.msg_type, dice_msg.group_id)][ClueInfo]);
				}
				else
				{
					/*
								ostringstream os1;
								bool a = ClueProp.count(ClueType(dice_msg.msg_type, dice_msg.group_id));
								os1 << a;
								ostringstream os2;
								bool b = (ClueProp[ClueType(
									dice_msg.msg_type, dice_msg.group_id)].count(ClueInfo));
								os2 << b;
								ʹ��os1.str()���ɻ�ý��
								*/
					dice_msg.Reply(GlobalMsg["strClueNotFound"]);
				}
				return;
			}
			bool boolError = false;
			string a = "";
			string b = "";
			while (intMsgCnt != strLowerMessage.length())
			{
				string ClueInfo = "Clue";
				string ClueMessages;
				while (intMsgCnt != strLowerMessage.length())
				{
					ClueMessages += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				a = ClueInfo;
				b = ClueMessages;
				if (ClueInfo.empty() || ClueMessages.empty())
				{
					boolError = true;
					break;
				}
				ClueProp[ClueType(dice_msg.msg_type, dice_msg.group_id)][ClueInfo] += "��Ϣ��¼��:" + strNickName + ",��Ϣ: " + ClueMessages + "\n";
			}
			if (boolError)
			{
				dice_msg.Reply("��������ʧ�ܣ��뽫��������˽������лл�� ClueInfo: " + a + ",ClueMessages: " + b);
			}
			else
			{
				dice_msg.Reply("�������óɹ��������������������Ϊû��ɾ�������Ĺ���");
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "ri")
		{
			if (dice_msg.msg_type == Dice::MsgType::Private)
			{
				dice_msg.Reply(GlobalMsg["strCommandNotAvailableErr"]);
				return;
			}
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strinit = "D20";
			if (strLowerMessage[intMsgCnt] == '+' || strLowerMessage[intMsgCnt] == '-')
			{
				strinit += strLowerMessage[intMsgCnt];
				intMsgCnt++;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
			}
			else if (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				strinit += '+';
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strinit += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			string strname = dice_msg.msg.substr(intMsgCnt);
			if (strname.empty())
				strname = strNickName;
			else
				strname = strip(strname);
			RD initdice(strinit);
			const int intFirstTimeRes = initdice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				dice_msg.Reply(GlobalMsg["strValueErr"]);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				dice_msg.Reply(GlobalMsg["strInputErr"]);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				dice_msg.Reply(GlobalMsg["strZeroDiceErr"]);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				dice_msg.Reply(GlobalMsg["strZeroTypeErr"]);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				dice_msg.Reply(GlobalMsg["strDiceTooBigErr"]);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				dice_msg.Reply(GlobalMsg["strTypeTooBigErr"]);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				dice_msg.Reply(GlobalMsg["strAddDiceValErr"]);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				dice_msg.Reply(GlobalMsg["strUnknownErr"]);
				return;
			}
			ilInitList->insert(dice_msg.group_id, initdice.intTotal, strname);
			const string strReply = strname + "���ȹ����㣺" + strinit + '=' + to_string(initdice.intTotal);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "team")
		{
		intMsgCnt += 4;
		string strReply;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
		if (strLowerMessage.substr(intMsgCnt, 3) == "add")
		{
			intMsgCnt += 3;
			strReply += "�µĳ�Ա�Ѿ����~��������";
			while (intMsgCnt != strLowerMessage.length())
			{
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
				if (strLowerMessage.substr(intMsgCnt, 10) == "[cq:at,qq=")
				{
					intMsgCnt += 10;
					string strNumber;
					long long PLnumber;

					while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					{
						strNumber += strLowerMessage[intMsgCnt];
						intMsgCnt++;
					}
					PLnumber = stoll(strNumber);
					TeamSaver.insert({ SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id), PLnumber });
					intMsgCnt += 1;
					strReply += "\n����Ա��" + getUname(SourceType(PLnumber, dice_msg.msg_type, dice_msg.group_id));
				}
				else
				{
					dice_msg.Reply(GlobalMsg["strTeamInsertErr"]);
					return;
				}
			}
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "era")
		{
			intMsgCnt += 3;
			strReply += "�ã��Ұ���Ӷ����л���:";
			while (intMsgCnt != strLowerMessage.length())
			{
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
				if (strLowerMessage.substr(intMsgCnt, 10) == "[cq:at,qq=")
				{
					intMsgCnt += 10;
					string strNumber;
					long long PLnumber;
					while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					{
						strNumber += strLowerMessage[intMsgCnt];
						intMsgCnt++;
					}
					PLnumber = stoll(strNumber);
					const auto cit = TeamSaver.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
					multimap<SourceType, long long>::iterator Mfinder = cit.first;
					for (; Mfinder != cit.second; ++Mfinder)
					{
						if (Mfinder->second == PLnumber)Mfinder = TeamSaver.erase(Mfinder);
					}
					intMsgCnt += 1;
					strReply += "\n����Ա:" + getUname(SourceType(PLnumber, dice_msg.msg_type, dice_msg.group_id));
				}
				else
				{
					TeamSaver.erase(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
					dice_msg.Reply("�Ѿ��������С���Ա");
					return;

				}
				dice_msg.Reply(strReply);
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "call")
		{
			intMsgCnt += 4;
			if (TeamSaver.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id))!=0)
			{
				string strNumber;
				const auto cit = TeamSaver.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
				multimap<SourceType, long long>::iterator Mfinder = cit.first;
				for (; Mfinder != cit.second;++Mfinder)
				{
					strReply += "[CQ:at,qq=" + to_string(Mfinder->second) + "]";
				}
			}
			else
			{
				dice_msg.Reply("���team��û�е���Ա��������ϸȷ��");
				return;
			}
			dice_msg.Reply(strReply);
		}
		else
		{
			if (TeamSaver.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) != 0)
			{
				strReply += "��������team�еĵ���Ա:";
				string strNumber;
				const auto cit = TeamSaver.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
				multimap<SourceType, long long>::iterator Mfinder = cit.first;
				for (; Mfinder != cit.second; ++Mfinder)
				{
					if (Usingname.count(SourceType(Mfinder->second, dice_msg.msg_type, dice_msg.group_id)))
						strReply += "\n"+ Usingname[SourceType(Mfinder->second, dice_msg.msg_type, dice_msg.group_id)] +":"+getDetail(SourceType(Mfinder->second, dice_msg.msg_type, dice_msg.group_id), Usingname[SourceType(Mfinder->second, dice_msg.msg_type, dice_msg.group_id)]);
					else strReply += "\n" + getName(Mfinder->second) + "(default):" +getDetail(SourceType(Mfinder->second, dice_msg.msg_type, dice_msg.group_id),"default");
				}
			}
			else
			{
				dice_msg.Reply("���team��û�е���Ա��������ϸȷ��");
				return;
			}
			dice_msg.Reply(strReply);
		}
		}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "init")
		{
			if (dice_msg.msg_type == Dice::MsgType::Private)
			{
				dice_msg.Reply(GlobalMsg["strCommandNotAvailableErr"]);
				return;
			}
			intMsgCnt += 4;
			string strReply;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (strLowerMessage.substr(intMsgCnt, 3) == "clr")
			{
				if (ilInitList->clear(dice_msg.group_id))
					strReply = GlobalMsg["strInitListClearedNotice"];
				else
					strReply = GlobalMsg["strInitListIsEmptyErr"];
				dice_msg.Reply(strReply);
				return;
			}
			ilInitList->show(dice_msg.group_id, strReply);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage[intMsgCnt] == 'w')
		{
			intMsgCnt++;
			bool boolDetail = false;
			if (strLowerMessage[intMsgCnt] == 'w')
			{
				intMsgCnt++;
				boolDetail = true;
			}
			bool isHidden = false;
			if (strLowerMessage[intMsgCnt] == 'h')
			{
				isHidden = true;
				intMsgCnt += 1;
			}
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;

			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != dice_msg.msg.length() && dice_msg.msg[intTmpMsgCnt] != ' ';
				intTmpMsgCnt++)
			{
				if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b' &&
					strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[intTmpMsgCnt] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a')
				{
					break;
				}
			}
			string strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intTmpMsgCnt])))
				intTmpMsgCnt++;
			string strReason = dice_msg.msg.substr(intTmpMsgCnt);

			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, dice_msg.qq_id);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					dice_msg.Reply(GlobalMsg["strValueErr"]);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					dice_msg.Reply(GlobalMsg["strInputErr"]);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					dice_msg.Reply(GlobalMsg["strZeroDiceErr"]);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					dice_msg.Reply(GlobalMsg["strZeroTypeErr"]);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					dice_msg.Reply(GlobalMsg["strDiceTooBigErr"]);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					dice_msg.Reply(GlobalMsg["strTypeTooBigErr"]);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					dice_msg.Reply(GlobalMsg["strAddDiceValErr"]);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					dice_msg.Reply(GlobalMsg["strUnknownErr"]);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					dice_msg.Reply(GlobalMsg["strRollTimeExceeded"]);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					dice_msg.Reply(GlobalMsg["strRollTimeErr"]);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";
					if (!isHidden)
					{
						dice_msg.Reply(strTurnNotice);
					}
					else
					{
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							strTurnNotice = "��Ⱥ\"" + getGroupList()[dice_msg.group_id] + "\"�� " + strTurnNotice;
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							strTurnNotice = "�ڶ��������� " + strTurnNotice;
						}
						AddMsgToQueue(Dice::DiceMsg(strTurnNotice, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
						pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							range = ObserveDiscuss.equal_range(dice_msg.group_id);
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							range = ObserveDiscuss.equal_range(dice_msg.group_id);
						}
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != dice_msg.qq_id)
							{
								AddMsgToQueue(Dice::DiceMsg(strTurnNotice, 0LL, it->second, Dice::MsgType::Private));
							}
						}
					}
				}
			}
			if (strMainDice.empty())
			{
				dice_msg.Reply(GlobalMsg["strEmptyWWDiceErr"]);
				return;
			}
			string strFirstDice = strMainDice.substr(0, strMainDice.find('+') < strMainDice.find('-')
				? strMainDice.find('+')
				: strMainDice.find('-'));
			bool boolAdda10 = true;
			for (auto i : strFirstDice)
			{
				if (!isdigit(static_cast<unsigned char>(i)))
				{
					boolAdda10 = false;
					break;
				}
			}
			if (boolAdda10)
				strMainDice.insert(strFirstDice.length(), "a10");
			RD rdMainDice(strMainDice, dice_msg.qq_id);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				dice_msg.Reply(GlobalMsg["strValueErr"]);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				dice_msg.Reply(GlobalMsg["strInputErr"]);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				dice_msg.Reply(GlobalMsg["strZeroDiceErr"]);
				return;
			}
			else
			{
				if (intFirstTimeRes == ZeroType_Err)
				{
					dice_msg.Reply(GlobalMsg["strZeroTypeErr"]);
					return;
				}
				if (intFirstTimeRes == DiceTooBig_Err)
				{
					dice_msg.Reply(GlobalMsg["strDiceTooBigErr"]);
					return;
				}
				if (intFirstTimeRes == TypeTooBig_Err)
				{
					dice_msg.Reply(GlobalMsg["strTypeTooBigErr"]);
					return;
				}
				if (intFirstTimeRes == AddDiceVal_Err)
				{
					dice_msg.Reply(GlobalMsg["strAddDiceValErr"]);
					return;
				}
				if (intFirstTimeRes != 0)
				{
					dice_msg.Reply(GlobalMsg["strUnknownErr"]);
					return;
				}
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				if (!isHidden)
				{
					dice_msg.Reply(strAns);
				}
				else
				{
					if (dice_msg.msg_type == Dice::MsgType::Group)
					{
						strAns = "��Ⱥ\"" + getGroupList()[dice_msg.group_id] + "\"�� " + strAns;
					}
					else if (dice_msg.msg_type == Dice::MsgType::Discuss)
					{
						strAns = "�ڶ��������� " + strAns;
					}
					AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
					pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
					if (dice_msg.msg_type == Dice::MsgType::Group)
					{
						range = ObserveDiscuss.equal_range(dice_msg.group_id);
					}
					else if (dice_msg.msg_type == Dice::MsgType::Discuss)
					{
						range = ObserveDiscuss.equal_range(dice_msg.group_id);
					}
					for (auto it = range.first; it != range.second; ++it)
					{
						if (it->second != dice_msg.qq_id)
						{
							AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, it->second, Dice::MsgType::Private));
						}
					}
				}
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail ? rdMainDice.FormCompleteString() : rdMainDice.FormShortString());
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					if (!isHidden)
					{
						dice_msg.Reply(strAns);
					}
					else
					{
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							strAns = "��Ⱥ\"" + getGroupList()[dice_msg.group_id] + "\"�� " + strAns;
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							strAns = "�ڶ��������� " + strAns;
						}
						AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
						pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							range = ObserveGroup.equal_range(dice_msg.group_id);
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							range = ObserveDiscuss.equal_range(dice_msg.group_id);
						}
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != dice_msg.qq_id)
							{
								AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, it->second, Dice::MsgType::Private));
							}
						}
					}
				}
			}
			if (isHidden)
			{
				const string strReply = strNickName + "������һ�ΰ���";
				dice_msg.Reply(strReply);
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "ob")
		{
			if (dice_msg.msg_type == Dice::MsgType::Private)
			{
				dice_msg.Reply(GlobalMsg["strCommandNotAvailableErr"]);
				return;
			}
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			const string Command = strLowerMessage.substr(intMsgCnt, dice_msg.msg.find(' ', intMsgCnt) - intMsgCnt);
			if (Command == "on")
			{
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
					{
						if (DisabledOBGroup.count(dice_msg.group_id))
						{
							DisabledOBGroup.erase(dice_msg.group_id);
							dice_msg.Reply(GlobalMsg["strObCommandSuccessfullyEnabledNotice"]);
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strObCommandAlreadyEnabledErr"]);
						}
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
					}
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					if (DisabledOBDiscuss.count(dice_msg.group_id))
					{
						DisabledOBDiscuss.erase(dice_msg.group_id);
						dice_msg.Reply(GlobalMsg["strObCommandSuccessfullyEnabledNotice"]);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strObCommandAlreadyEnabledErr"]);
					}
				}

				return;
			}
			if (Command == "off")
			{
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
					{
						if (!DisabledOBGroup.count(dice_msg.group_id))
						{
							DisabledOBGroup.insert(dice_msg.group_id);
							ObserveGroup.erase(dice_msg.group_id);
							dice_msg.Reply(GlobalMsg["strObCommandSuccessfullyDisabledNotice"]);
						}
						else
						{
							dice_msg.Reply(GlobalMsg["strObCommandAlreadyDisabledErr"]);
						}
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
					}
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					if (!DisabledOBDiscuss.count(dice_msg.group_id))
					{
						DisabledOBDiscuss.insert(dice_msg.group_id);
						ObserveDiscuss.erase(dice_msg.group_id);
						dice_msg.Reply(GlobalMsg["strObCommandSuccessfullyDisabledNotice"]);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strObCommandAlreadyDisabledErr"]);
					}
				}
				return;
			}
			if ((dice_msg.msg_type == Dice::MsgType::Group && DisabledOBGroup.count(dice_msg.group_id)) || (dice_msg.msg_type == Dice::MsgType::Discuss && DisabledOBDiscuss.count(dice_msg.group_id)))
			{
				dice_msg.Reply(GlobalMsg["strObCommandDisabledErr"]);
				return;
			}
			if (Command == "list")
			{
				string Msg = "��ǰ���Թ�����:";
				pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					range = ObserveGroup.equal_range(dice_msg.group_id);
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					range = ObserveDiscuss.equal_range(dice_msg.group_id);
				}
				for (auto it = range.first; it != range.second; ++it)
				{
					Msg += "\n" + getName(it->second, dice_msg.group_id) + "(" + to_string(it->second) + ")";
				}
				const string strReply = Msg == "��ǰ���Թ�����:" ? "��ǰ�����Թ���" : Msg;
				dice_msg.Reply(strReply);
			}
			else if (Command == "clr")
			{
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
					{
						ObserveGroup.erase(dice_msg.group_id);
						dice_msg.Reply(GlobalMsg["strObListClearedNotice"]);
					}
					else
					{
						dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
					}
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					ObserveDiscuss.erase(dice_msg.group_id);
					dice_msg.Reply(GlobalMsg["strObListClearedNotice"]);
				}
			}
			else if (Command == "exit")
			{
				pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					range = ObserveGroup.equal_range(dice_msg.group_id);
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					range = ObserveDiscuss.equal_range(dice_msg.group_id);
				}
				for (auto it = range.first; it != range.second; ++it)
				{
					if (it->second == dice_msg.qq_id)
					{
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							ObserveGroup.erase(it);
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							ObserveDiscuss.erase(it);
						}
						const string strReply = strNickName + "�ɹ��˳��Թ�ģʽ!";
						dice_msg.Reply(strReply);
						return;
					}
				}
				const string strReply = strNickName + "û�м����Թ�ģʽ!";
				dice_msg.Reply(strReply);
			}
			else
			{
				pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					range = ObserveGroup.equal_range(dice_msg.group_id);
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					range = ObserveDiscuss.equal_range(dice_msg.group_id);
				}
				for (auto it = range.first; it != range.second; ++it)
				{
					if (it->second == dice_msg.qq_id)
					{
						const string strReply = strNickName + "�Ѿ������Թ�ģʽ!";
						dice_msg.Reply(strReply);
						return;
					}
				}
				if (dice_msg.msg_type == Dice::MsgType::Group)
				{
					ObserveGroup.insert(make_pair(dice_msg.group_id, dice_msg.qq_id));
				}
				else if (dice_msg.msg_type == Dice::MsgType::Discuss)
				{
					ObserveDiscuss.insert(make_pair(dice_msg.group_id, dice_msg.qq_id));
				}

				const string strReply = strNickName + "�ɹ������Թ�ģʽ!";
				dice_msg.Reply(strReply);
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "ti")
		{
			string strAns = strNickName + "�ķ����-��ʱ֢״:\n";
			Log->record(dice_msg.group_id, "", strAns);
			TempInsane(strAns);
			dice_msg.Reply(strAns);
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "li")
		{
			string strAns = strNickName + "�ķ����-�ܽ�֢״:\n";
			Log->record(dice_msg.group_id, "", strAns);
			LongInsane(strAns);
			dice_msg.Reply(strAns);
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "en" && strLowerMessage.substr(intMsgCnt, 3) != "end")
	{
	intMsgCnt += 2;
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	string strSkillName;
	while (intMsgCnt != dice_msg.msg.length() && !isdigit(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])) && !isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt]))
		)
	{
		strSkillName += strLowerMessage[intMsgCnt];
		intMsgCnt++;
	}
	if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
	while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
		intMsgCnt++;
	string strCurrentValue;
	while (isdigit(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
	{
		strCurrentValue += dice_msg.msg[intMsgCnt];
		intMsgCnt++;
	}
	int intCurrentVal;
	bool found = 0;
	if(Cname !="default")found = ClistFounder(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id),Cname);
	if (strCurrentValue.length() > 3 && strCurrentValue.empty() != 1)
	{
		dice_msg.Reply(GlobalMsg["strEnValInvalid"]);
		return;
	}
	else if (strCurrentValue.empty() != 1)
	{
		intCurrentVal = stoi(strCurrentValue);
	}
	if (strCurrentValue.empty() && Cname == "default")
	{
		if (CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].count(strSkillName))
		{
			intCurrentVal = CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName];
		}
		else if (SkillDefaultVal.count(strSkillName))
		{
			intCurrentVal = SkillDefaultVal[strSkillName];
		}
		else
		{
			dice_msg.Reply(GlobalMsg["strUnknownPropErr"]);
			return;
		}
	}
	else if (strCurrentValue.empty())
	{
		if(found == 0)
		{
			dice_msg.Reply(GlobalMsg["strCEmpty"]);
			return;
		}
		if (F2Fsk->second.count(strSkillName))intCurrentVal = F2Fsk->second[strSkillName];
		else if (SkillDefaultVal.count(strSkillName))
		{
			intCurrentVal = SkillDefaultVal[strSkillName];
		}
		else
		{
			dice_msg.Reply(GlobalMsg["strUnknownPropErr"]);
			return;
		}
	}
	string strAns = strNickName + "��" + strSkillName + "��ǿ��ɳ��춨:\n1D100=";
	const int intTmpRollRes = RandomGenerator::Randint(1, 100);
	strAns += to_string(intTmpRollRes) + "/" + to_string(intCurrentVal);

	if (intTmpRollRes <= intCurrentVal && intTmpRollRes <= 95)
	{
		strAns += " ʧ��!\n���" + (strSkillName.empty() ? "���Ի���ֵ" : strSkillName) + "û�б仯!";
	}
	else
	{
		strAns += " �ɹ�!\n���" + (strSkillName.empty() ? "���Ի���ֵ" : strSkillName) + "����1D10=";
		const int intTmpRollD10 = RandomGenerator::Randint(1, 10);
		strAns += to_string(intTmpRollD10) + "��,��ǰΪ" + to_string(intCurrentVal + intTmpRollD10) + "��";
		if (strCurrentValue.empty() && Cname == "default")
		{
			CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName] = intCurrentVal +
				intTmpRollD10;
		}
		else if (strCurrentValue.empty())
		{
			F2Fsk->second[strSkillName] = intCurrentVal + intTmpRollD10;
		}
	}
	dice_msg.Reply(strAns);
	}
	    else if (strLowerMessage.substr(intMsgCnt, 2) == "sc")
	{
	intMsgCnt += 2;
	const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
	auto ENfinder1 = range.first;
	bool found = 0;
	if (Cname != "default")
	{
	found = ClistFounder(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id), Cname);
	}
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	string SanCost = strLowerMessage.substr(intMsgCnt, dice_msg.msg.find(' ', intMsgCnt) - intMsgCnt);
	intMsgCnt += SanCost.length();
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		intMsgCnt++;
	string San;
	while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
	{
		San += strLowerMessage[intMsgCnt];
		intMsgCnt++;
	}
	if (SanCost.empty() || SanCost.find("/") == string::npos)
	{
		dice_msg.Reply(GlobalMsg["strSCInvalid"]);
		return;
	}
	if (Cname == "default" && San.empty() && !(CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CharacterProp[
		SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].count("����")))
	{
		dice_msg.Reply(GlobalMsg["strSanInvalid"]);
		return;
	}
	else if (Cname != "default" && San.empty() && F2Fsk->second.count("����") != 1)
	{
		dice_msg.Reply(GlobalMsg["strCSanInvalid"]);
		return;
	}
	for (const auto& character : SanCost.substr(0, SanCost.find("/")))
	{
		if (!isdigit(static_cast<unsigned char>(character)) && character != 'D' && character != 'd' && character != '+' && character != '-')
		{
			dice_msg.Reply(GlobalMsg["strSCInvalid"]);
			return;
		}
	}
	for (const auto& character : SanCost.substr(SanCost.find("/") + 1))
	{
		if (!isdigit(static_cast<unsigned char>(character)) && character != 'D' && character != 'd' && character != '+' && character != '-')
		{
			dice_msg.Reply(GlobalMsg["strSCInvalid"]);
			return;
		}
	}
	RD rdSuc(SanCost.substr(0, SanCost.find("/")));
	RD rdFail(SanCost.substr(SanCost.find("/") + 1));
	if (rdSuc.Roll() != 0 || rdFail.Roll() != 0)
	{
		dice_msg.Reply(GlobalMsg["strSCInvalid"]);
		return;
	}
	if (San.length() >= 3)
	{
		dice_msg.Reply(GlobalMsg["strSanInvalid"]);
		return;
	}
	int intSan = 0;
	if (Cname == "default" && San.empty())
		intSan = CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)]["����"];
	else if (Cname != "default" && San.empty())
		intSan = F2Fsk->second["����"];
	else
		intSan = stoi(San);

	if (intSan == 0)
	{
		dice_msg.Reply(GlobalMsg["strSanInvalid"]);
		return;
	}	
	string strAns = strNickName + "��Sancheck:\n1D100=";
	const int intTmpRollRes = RandomGenerator::Randint(1, 100);
	strAns += to_string(intTmpRollRes);
	if (intTmpRollRes <= intSan)
	{
		strAns += "  �ðɺðɣ�������־�ᶨ���ɹ���\n���Sanֵ����" + SanCost.substr(0, SanCost.find("/"));
		if (SanCost.substr(0, SanCost.find("/")).find("d") != string::npos)
			strAns += "=" + to_string(rdSuc.intTotal);
		strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdSuc.intTotal)) + "�㣬�´��ҿɲ������˰�";
		if (San.empty() && Cname == "default")	CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)]["����"] = max(0, intSan - rdSuc.intTotal);
		else if (San.empty() && Cname != "default") F2Fsk->second["����"] = max(0, intSan - rdSuc.intTotal);
	}
	else if (intTmpRollRes == 100 || (intSan < 50 && intTmpRollRes > 95))
	{
		strAns += "  ������ʧ�ܣ��������㣬\n���Sanֵ����" + SanCost.substr(SanCost.find("/") + 1);
		// ReSharper disable once CppExpressionWithoutSideEffects
		rdFail.Max();
		if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos)
			strAns += "���ֵ=" + to_string(rdFail.intTotal);
		strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdFail.intTotal)) + "��";
		if (San.empty() && Cname == "default")	CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)]["����"] = max(0, intSan - rdFail.intTotal);
		else if (San.empty() && Cname != "default") F2Fsk->second["����"] = max(0, intSan - rdFail.intTotal);
	}
	else
	{
		strAns += " ����ʧ���ˣ�����Ӧ������Ӧ��\n���Sanֵ����" + SanCost.substr(SanCost.find("/") + 1);
		if (SanCost.substr(SanCost.find("/") + 1).find("d") != string::npos)
			strAns += "=" + to_string(rdFail.intTotal);
		strAns += +"��,��ǰʣ��" + to_string(max(0, intSan - rdFail.intTotal)) + "�㣬����û��";
		if (San.empty() && Cname == "default")	CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)]["����"] = max(0, intSan - rdFail.intTotal);
		else if (San.empty() && Cname != "default") F2Fsk->second["����"] = max(0, intSan - rdFail.intTotal);
	}
	dice_msg.Reply(strAns);
	}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "jrrp")
	{
	if (dice_msg.msg_type == Dice::MsgType::Group)
	{
		intMsgCnt += 4;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		const string Command = strLowerMessage.substr(intMsgCnt, strLowerMessage.find(' ', intMsgCnt) - intMsgCnt);
		if (Command == "on")
		{
			if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
			{
				if (DisabledJRRPGroup.count(dice_msg.group_id))
				{
					DisabledJRRPGroup.erase(dice_msg.group_id);
					dice_msg.Reply("�ɹ��ڱ�Ⱥ������JRRP!");
				}
				else
				{
					dice_msg.Reply("�ڱ�Ⱥ��JRRPû�б�����!");
				}
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
			}
			return;
		}
		if (Command == "off")
		{
			if (getGroupMemberInfo(dice_msg.group_id, dice_msg.qq_id).permissions >= 2)
			{
				if (!DisabledJRRPGroup.count(dice_msg.group_id))
				{
					DisabledJRRPGroup.insert(dice_msg.group_id);
					dice_msg.Reply("�ɹ��ڱ�Ⱥ�н���JRRP!");
				}
				else
				{
					dice_msg.Reply("�ڱ�Ⱥ��JRRPû�б�����!");
				}
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strPermissionDeniedErr"]);
			}
			return;
		}
		if (DisabledJRRPGroup.count(dice_msg.group_id))
		{
			dice_msg.Reply("�ڱ�Ⱥ��JRRP�����ѱ�����");
			return;
		}
	}
	else if (dice_msg.msg_type == Dice::MsgType::Discuss)
	{
		intMsgCnt += 4;
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			intMsgCnt++;
		const string Command = strLowerMessage.substr(intMsgCnt, strLowerMessage.find(' ', intMsgCnt) - intMsgCnt);
		if (Command == "on")
		{
		
				if (DisabledJRRPDiscuss.count(dice_msg.group_id))
				{
					DisabledJRRPDiscuss.erase(dice_msg.group_id);
					dice_msg.Reply("�ɹ��ڱ�Ⱥ������JRRP!");
				}
				else
				{
					dice_msg.Reply("�ڱ�Ⱥ��JRRPû�б�����!");
				}
			return;
		} 
		if (Command == "off")
		{
			
				if (!DisabledJRRPDiscuss.count(dice_msg.group_id))
				{
					DisabledJRRPDiscuss.insert(dice_msg.group_id);
					dice_msg.Reply("�ɹ��ڱ��������н���JRRP!");
				}
				else
				{
					dice_msg.Reply("�ڱ���������JRRPû�б�����!");
				}
			
			return;
		}
	}
	string des;
	int timeN = (time(NULL) / 86400) % 50;
	int jrrp = (9 * timeN * (getLoginQQ() % 1000 + dice_msg.qq_id % 1000 + timeN * 11) + 77) % 100;
	if (jrrp == 0)jrrp = 100;
	if (jrrp >= 50)dice_msg.Reply(format(GlobalMsg["strJrrp"], { strNickName, to_string(jrrp - 50) }));
	else dice_msg.Reply(format(GlobalMsg["strJrrpsuc"], { strNickName, to_string(50 - jrrp) }));
	}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "name")
		{
			intMsgCnt += 4;
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;

			string type;
			while (isalpha(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				type += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}

			auto nameType = NameGenerator::Type::UNKNOWN;
			if (type == "cn")
				nameType = NameGenerator::Type::CN;
			else if (type == "en")
				nameType = NameGenerator::Type::EN;
			else if (type == "jp")
				nameType = NameGenerator::Type::JP;

			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;

			string strNum;
			while (isdigit(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
			{
				strNum += dice_msg.msg[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.size() > 2)
			{
				dice_msg.Reply(GlobalMsg["strNameNumTooBig"]);
				return;
			}
			int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				dice_msg.Reply(GlobalMsg["strNameNumTooBig"]);
				return;
			}
			if (intNum == 0)
			{
				dice_msg.Reply(GlobalMsg["strNameNumCannotBeZero"]);
				return;
			}
			vector<string> TempNameStorage;
			while (TempNameStorage.size() != intNum)
			{
				string name = NameGenerator::getRandomName(nameType);
				if (find(TempNameStorage.begin(), TempNameStorage.end(), name) == TempNameStorage.end())
				{
					TempNameStorage.push_back(name);
				}
			}
			string strReply = strNickName + "���������:\n";
			for (auto i = 0; i != TempNameStorage.size(); i++)
			{
				strReply.append(TempNameStorage[i]);
				if (i != TempNameStorage.size() - 1)
					strReply.append(", ");
			}
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "nnn")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;
			string type = strLowerMessage.substr(intMsgCnt, 2);
			string name;
			if (type == "cn")
				name = NameGenerator::getChineseName();
			else if (type == "en")
				name = NameGenerator::getEnglishName();
			else if (type == "jp")
				name = NameGenerator::getJapaneseName();
			else
				name = NameGenerator::getRandomName();
			Name->set(dice_msg.group_id, dice_msg.qq_id, name);
			const string strReply = "Ŷ~" + strNickName + "�Ľ�" + name + "�ˣ��Ե��Ѽ�һ�£�";
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "nn")
		{
			intMsgCnt += 2;
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;
			string name = dice_msg.msg.substr(intMsgCnt);
			if (name.length() > 50)
			{
				dice_msg.Reply(GlobalMsg["strNameTooLongErr"]);
				return;
			}
			if (!name.empty())
			{
				Name->set(dice_msg.group_id, dice_msg.qq_id, name);
				const string strReply = "Ŷ~" + strNickName + "�Ľ�" + strip(name) + "�ˣ��Ե��Ѽ�һ�£�";
				dice_msg.Reply(strReply);
			}
			else
			{
				if (Name->del(dice_msg.group_id, dice_msg.qq_id))
				{
					const string strReply = "�Ѻ�������" + strNickName + "��ʲô�ˡ���������ʲô��˼���ѲŲ��Ǳ�����";
					dice_msg.Reply(strReply);
				}
				else
				{
					const string strReply = strNickName + GlobalMsg["strNameDelErr"];
					dice_msg.Reply(strReply);
				}
			}
		}
		else if (strLowerMessage[intMsgCnt] == 'n' && strLowerMessage.substr(intMsgCnt, 3) != "npc")
		{
			intMsgCnt += 1;
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;
			string name = dice_msg.msg.substr(intMsgCnt);
			if (name.length() > 50)
			{
				dice_msg.Reply(GlobalMsg["strNameTooLongErr"]);
				return;
			}
			if (!name.empty())
			{
				Name->set(0LL, dice_msg.qq_id, name);
				const string strReply = "����ÿ��Ⱥ���Ľ�" + strip(name) + "�𣿰������ö�Ⱥ����";
				dice_msg.Reply(strReply);
			}
			else
			{
				if (Name->del(0LL, dice_msg.qq_id))
				{
					const string strReply = "ȫ����ȫ����������Եȣ�";
					dice_msg.Reply(strReply);
				}
				else
				{
					const string strReply = strNickName + GlobalMsg["strNameDelErr"];
					dice_msg.Reply(strReply);
				}
			}
		}
		else if (0&&(strLowerMessage.substr(intMsgCnt, 3) == "log" | strLowerMessage.substr(intMsgCnt, 5) == "begin" | strLowerMessage.substr(intMsgCnt, 3) == "end" | (strLowerMessage.substr(intMsgCnt, 3) == "get" & strLowerMessage.substr(intMsgCnt, 7) != "getcard" & strLowerMessage.substr(intMsgCnt, 7) != "getbook")))
		{
			string Command;
			if (strLowerMessage.substr(intMsgCnt, 3) == "log")
			{
				intMsgCnt += 3;
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;

				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
				if (strLowerMessage.substr(intMsgCnt, 2) == "on")
				{
					Command = "on";
					intMsgCnt += 2;
				}
				else if (strLowerMessage.substr(intMsgCnt, 3) == "off")
				{
					Command = "off";
					intMsgCnt += 3;
				}
				else if (strLowerMessage.substr(intMsgCnt, 3) == "get")
				{
					Command = "get";
					intMsgCnt += 3;
				}
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
			}
			if (strLowerMessage.substr(intMsgCnt, 5) == "begin")
			{
				intMsgCnt += 5;
				Command = "on";
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "end")
			{
				intMsgCnt += 3;
				Command = "off";
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
			}
			if (strLowerMessage.substr(intMsgCnt, 3) == "get")
			{
				intMsgCnt += 3;
				Command = "get";
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
					intMsgCnt++;
			}
			if (Command == "on")
			{
				if (!Log->haveAuthority(dice_msg.qq_id, dice_msg.group_id))
				{
					dice_msg.Reply("������Ч�����������û��ڱ�Ⱥ����log��¼����ȴ��䣨ʹ��.log off��.end��������ϡ�");
					return;
				}
				string name = dice_msg.msg.substr(intMsgCnt, dice_msg.msg.find(' ', intMsgCnt) - intMsgCnt);
				if (name.length() > 50)
				{
					dice_msg.Reply(GlobalMsg["strNameTooLongErr"]);
					return;
				}
				if (!name.empty())
				{
					time_t myTime = time(NULL);
					string savePath = "C:\\Files\\" + name + "." + to_string(dice_msg.group_id) + ".txt";
					string savePathbak = "C:\\Files\\" + name + "." + to_string(dice_msg.group_id) + ".txt" + to_string(myTime);
					if (!rename(savePath.c_str(), savePathbak.c_str()))
					{
						dice_msg.Reply("�Ѵ���ͬ��Log�ļ�" + name + "." + to_string(dice_msg.group_id) + ".txt������Ϊ" + name + "." + to_string(dice_msg.group_id) + ".txt" + to_string(myTime));
					}
					Log->logOn(dice_msg.qq_id, dice_msg.group_id, name);
					const string strReply = "�ѿ�ʼΪ " + strip(name) + " ��¼log�������������ؼ�סŶ��ȡlogʱҪ�õġ�\nʹ�ù淶:\n������#��ͷ\n�Ի�����������������Ӣ�ľ��ɣ�\n���⻰�����Ű�������Ӣ�ľ��ɣ�";
					dice_msg.Reply(strReply);
				}
				else
				{
					dice_msg.Reply("������log����");
				}
			}
			if (Command == "off")
			{
				if (!Log->haveAuthority(dice_msg.qq_id, dice_msg.group_id))
				{
					dice_msg.Reply("������Ч�����������û��ڱ�Ⱥ����log��¼����ȴ��䣨ʹ��.log off��.end��������ϡ�");
					return;
				}
				if (Log->logOff(dice_msg.group_id))
				{
					const string strReply = "����ֹlog��¼";
					dice_msg.Reply(strReply);
				}
				else
				{
					const string strReply = "��Чָ��";
					dice_msg.Reply(strReply);
				}
			}
			if (Command == "get")
			{
				if (!Log->haveAuthority(dice_msg.qq_id, dice_msg.group_id))
				{
					dice_msg.Reply("������Ч�����������û��ڱ�Ⱥ����log��¼����ȴ��䣨ʹ��.log off��.end��������ϡ�");
					return;
				}
				string name = dice_msg.msg.substr(intMsgCnt, dice_msg.msg.find(' ', intMsgCnt) - intMsgCnt);
				if (name.length() > 50)
				{
					dice_msg.Reply(GlobalMsg["strNameTooLongErr"]);
					return;
				}
				int tmpETR = emailTimeRemain(dice_msg.qq_id);
				if (tmpETR <= 0)
				{
					dice_msg.Reply(GlobalMsg["strNoExtractionTime"]);
					return;
				}
				else
				{
					dice_msg.Reply("������ĳ�ȡ������ʣ��" + to_string(tmpETR - 1) + "��");
					emailTimeReduce(dice_msg.qq_id);
				}
				if (!name.empty())
				{
					dice_msg.Reply("���ڴ����й� " + name + " ��log�����Ե�");
					/*string logGet = Log->get(dice_msg.qq_id, dice_msg.group_id, name);
						if (logGet == "\n")
						{
							dice_msg.Reply("����ؼ�¼");
							return;
						}*/

					string fileName = string(name + "." + changetoString(dice_msg.group_id) + ".txt");
					string filePath = string("C:\\Files\\" + fileName);

					//string fileNameDocx = string(name + "." + changetoString(dice_msg.qq_id) + ".docx");
					//string filePathDocx = string("C:/Files/" + fileNameDocx);

					Sleep(5000);

					dice_msg.Reply(string("�����ϣ�׼�����͵�����: " + changetoString(dice_msg.qq_id) + "@qq.com"));
					CSmtp smtp(
						25,																											  /*smtp�˿�*/
						"smtp.qq.com",																								  /*smtp��������ַ*/
						"1714545141@qq.com",																							  /*��������ַ*/
						"ivctxepsrqofbccf",																							  /*��������*/
						changetoString(dice_msg.qq_id) + "@qq.com",																	  /*Ŀ�������ַ,��һ�����ÿո�ָ����Ӷ��������*/
						"����log�ļ�:" + name,																						  /*����*/
						"������" + fileName + "��Ϊlog��¼������ػظ�һ�¸��ʼ�����Ȼ�Ժ�ϵͳ����ѷ����ʼ����ൽ�����ʼ��ġ�o(�i�n�i)o" /*�ʼ�����*/
					);
					/**
						//��Ӹ���ʱע��,\һ��Ҫд��\\����Ϊת���ַ���Ե��
						string filePath("D:\\�γ���Ʊ���.doc");
						smtp.AddAttachment(filePath);
						*/

					smtp.AddAttachment(filePath); //��Ӹ���

					int err;
					if ((err = smtp.SendEmail_Ex()) != 0)
					{
						if (err == 1)
							dice_msg.Reply("����1: �������粻��ͨ������ʧ��!");
						if (err == 2)
							dice_msg.Reply("����2: �û�������,��˶�!");
						if (err == 3)
							dice_msg.Reply("����3: �û����������˶�!");
						if (err == 4)
							dice_msg.Reply("����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!");
					}
					else
					{
						dice_msg.Reply("�ѷ���log�ļ���" + strNickName + "��QQ���䣬��δ�������������䡣" + "������" + fileName + "��Ϊlog��¼��\n\n��С�����ػظ�һ�±��ʼ�����Ȼϵͳ�����װ��ѷ����ʼ����ൽ�����ʼ���\n�������ʲô������������ӭ���ʼ�������Ӵ~\n֪ͨ:���ڼ������⣬Ŀǰ����ʼ�������Ҫ�ӽ�15����ʱ��ſ��Է�����������У������ĵȴ������������Ų��С�");
					}
				}
				else
				{
					int tmpETR = emailTimeRemain(dice_msg.qq_id);
					if (tmpETR <= 0)
					{
						dice_msg.Reply(GlobalMsg["strNoExtractionTime"]);
						return;
					}
					else
					{
						dice_msg.Reply("������ĳ�ȡ������ʣ��" + to_string(tmpETR - 1) + "��");
						emailTimeReduce(dice_msg.qq_id);
					}
					dice_msg.Reply("���ڴ���" + strNickName + "��log�����Ե�");
					string logGet = Log->get(dice_msg.qq_id, dice_msg.group_id);
					if (logGet == "\n")
					{
						dice_msg.Reply("����ؼ�¼");
						return;
					}
					ofstream ost;
					string fileName = string("all." + changetoString(dice_msg.qq_id) + ".txt");
					string filePath = string("C:\\Files\\" + fileName);
					string fileNameDocx = string("all." + changetoString(dice_msg.qq_id) + ".docx");
					string filePathDocx = string("C:\\Files\\" + fileNameDocx);

					Sleep(5000);

					dice_msg.Reply(string("�����ϣ�׼�����͵�����: " + changetoString(dice_msg.qq_id) + "@qq.com"));
					CSmtp smtp(
						25,																											  /*smtp�˿�*/
						"smtp.qq.com",																								  /*smtp��������ַ*/
						"450609203@qq.com",																							  /*��������ַ*/
						"ixnyvgacyfytbjfh",																							  /*��������*/
						changetoString(dice_msg.qq_id) + "@qq.com",																	  /*Ŀ�������ַ,��һ�����ÿո�ָ����Ӷ��������*/
						"����log�ļ�:" + name,																						  /*����*/
						"������" + fileName + "��Ϊlog��¼������ػظ�һ�¸��ʼ�����Ȼ�Ժ�ϵͳ����ѷ����ʼ����ൽ�����ʼ��ġ�o(�i�n�i)o" /*�ʼ�����*/
					);
					/**
						//��Ӹ���ʱע��,\һ��Ҫд��\\����Ϊת���ַ���Ե��
						string filePath("D:\\�γ���Ʊ���.doc");
						smtp.AddAttachment(filePath);
						*/

					WIN32_FIND_DATA FindFileData;
					HANDLE hFind;
					CString strFileName = CString((filePathDocx + ".finish").c_str());
					LPCWSTR lpcwStr = strFileName.AllocSysString();
					hFind = FindFirstFile(lpcwStr, &FindFileData);
					int sleepTime = 0;
					while ((hFind == INVALID_HANDLE_VALUE) & (sleepTime <= 10000))
					{
						hFind = FindFirstFile(lpcwStr, &FindFileData);
						Sleep(50);
						sleepTime += 1;
						if (sleepTime == 10000)
						{
							dice_msg.Reply("��ǸĿǰ���Ӻ�æ��δ��ʱ����Ⱦɫ����ļ������Ժ����ԡ�");
						}
					}
					if (hFind != INVALID_HANDLE_VALUE)
					{
						remove((filePathDocx + ".finish").c_str());
						dice_msg.Reply("������Ⱦɫ����ļ����������ڱȽ�æ���ļ�����5����͡������û�յ���˽���ѣ�");
						Sleep(5000);
					}

					smtp.AddAttachment(filePath); //��Ӹ���

					int err;
					if ((err = smtp.SendEmail_Ex()) != 0)
					{
						if (err == 1)
							dice_msg.Reply("����1: �������粻��ͨ������ʧ��!");
						if (err == 2)
							dice_msg.Reply("����2: �û�������,��˶�!");
						if (err == 3)
							dice_msg.Reply("����3: �û����������˶�!");
						if (err == 4)
							dice_msg.Reply("����4: ���鸽��Ŀ¼�Ƿ���ȷ���Լ��ļ��Ƿ����!");
					}
					else
					{
						dice_msg.Reply("�ѷ���log�ļ���" + strNickName + "��QQ���䣬��δ�������������䡣" + "������" + fileName + "��Ϊlog��¼��\n\n��С�����ػظ�һ�±��ʼ�����Ȼϵͳ�����װ��ѷ����ʼ����ൽ�����ʼ���\n�������ʲô������������ӭ���ʼ�������Ӵ~\n֪ͨ:���ڼ������⣬Ŀǰ����ʼ�������Ҫ�ӽ�15����ʱ��ſ��Է�����������У������ĵȴ������������Ų��С�");
					}
				}
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 5) == "rules")
		{
			intMsgCnt += 5;
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;
			string strSearch = dice_msg.msg.substr(intMsgCnt);
			for (auto &n : strSearch)
				n = toupper(static_cast<unsigned char>(n));
			string strReturn;
			if (GetRule::analyze(strSearch, strReturn))
			{
				dice_msg.Reply(strReturn);
			}
			else
			{
				dice_msg.Reply(GlobalMsg["strRuleErr"] + strReturn);
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "set")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strDefaultDice = strLowerMessage.substr(intMsgCnt, strLowerMessage.find(" ", intMsgCnt) - intMsgCnt);
			while (strDefaultDice[0] == '0')
				strDefaultDice.erase(strDefaultDice.begin());
			if (strDefaultDice.empty())
				strDefaultDice = "100";
			for (auto charNumElement : strDefaultDice)
				if (!isdigit(static_cast<unsigned char>(charNumElement)))
				{
					dice_msg.Reply(GlobalMsg["strSetInvalid"]);
					return;
				}
			if (strDefaultDice.length() > 5)
			{
				dice_msg.Reply(GlobalMsg["strSetTooBig"]);
				return;
			}
			const int intDefaultDice = stoi(strDefaultDice);
			if (intDefaultDice == 100)
				DefaultDice.erase(dice_msg.qq_id);
			else
				DefaultDice[dice_msg.qq_id] = intDefaultDice;
			const string strSetSuccessReply = "�ѽ�" + strNickName + "��Ĭ�������͸���ΪD" + strDefaultDice;
			dice_msg.Reply(strSetSuccessReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 5) == "coc6d")
		{
			string strReply = strNickName;
			COC6D(strReply);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 4) == "coc6")
		{
			intMsgCnt += 4;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			if (intNum == 0)
			{
				dice_msg.Reply(GlobalMsg["strCharacterCannotBeZero"]);
				return;
			}
			string strReply = strNickName;
			COC6(strReply, intNum);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "dnd")
		{
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			if (intNum == 0)
			{
				dice_msg.Reply(GlobalMsg["strCharacterCannotBeZero"]);
				return;
			}
			string strReply = strNickName;
			DND(strReply, intNum);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 5) == "coc7d" || strLowerMessage.substr(intMsgCnt, 4) == "cocd")
		{
			string strReply = strNickName;
			COC7D(strReply);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "coc")
		{
			intMsgCnt += 3;
			if (strLowerMessage[intMsgCnt] == '7')
				intMsgCnt++;
			if (strLowerMessage[intMsgCnt] == 's')
				intMsgCnt++;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			string strNum;
			while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				strNum += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			if (strNum.length() > 2)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			const int intNum = stoi(strNum.empty() ? "1" : strNum);
			if (intNum > 10)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			if (intNum == 0)
			{
				dice_msg.Reply(GlobalMsg["strCharacterCannotBeZero"]);
				return;
			}
			string strReply = strNickName;
			COC7(strReply, intNum);
			dice_msg.Reply(strReply);
		}
		else if (strLowerMessage.substr(intMsgCnt, 3) == "npc")
		{
			auto nameType = NameGenerator::Type::UNKNOWN;
			string name;
			vector<string> TempNameStorage;
			name = NameGenerator::getRandomName(nameType);
			intMsgCnt += 3;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			int intNum = stoi("1");
			if (intNum > 10)
			{
				dice_msg.Reply(GlobalMsg["strCharacterTooBig"]);
				return;
			}
			if (intNum == 0)
			{
				dice_msg.Reply(GlobalMsg["strCharacterCannotBeZero"]);
				return;
			}
			string strReply = strNickName;
			NPCGroup(strReply);
			dice_msg.Reply(strReply);
			string strReplayKP = name;
			NPCKP(strReplayKP);
			AddMsgToQueue(Dice::DiceMsg(strReplayKP, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
		}
		else if (strLowerMessage.substr(intMsgCnt, 7) == "getbook")
		{
			string Command = "";

			intMsgCnt += 7;
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;

			while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				Command += strLowerMessage[intMsgCnt];
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				intMsgCnt++;
			if (Command == "card")
			{
				dice_msg.Reply(GlobalMsg["strGetBookHumanMailContent"]);
			}
			else if (Command == "rp")
			{
				dice_msg.Reply(GlobalMsg["strGetBookRPMailContent"]);
			}
			else if (Command == "kp")
			{
				dice_msg.Reply(GlobalMsg["strGetBookKPMailContent"]);
				dice_msg.Reply("�����ṩ�������ذ棬��������������¼���ɲ鿴��\n���ӣ�https://pan.baidu.com/s/1LziIgr4Y8DCKlGa5Z8QVQQ ��ȡ�룺4d07");
			}
			else if (Command == "make")
			{
				dice_msg.Reply(GlobalMsg["strGetBookMakeCardMailContent"]);
				dice_msg.Reply("�����ṩ�������ذ棬��������������¼���ɲ鿴��\n���ӣ�https://pan.baidu.com/s/1HHo1B1F9kMRIFDB8J9Ulww ��ȡ�룺ql5h");
			}
		}
		else if (strLowerMessage.substr(intMsgCnt, 2) == "ra" || strLowerMessage.substr(intMsgCnt, 2) == "re" || strLowerMessage.substr(intMsgCnt, 2) == "rc")
	{
	int Rrolltype;
	if (strLowerMessage.substr(intMsgCnt, 2) == "ra")Rrolltype = 1;
	else if (strLowerMessage.substr(intMsgCnt, 2) == "rc")Rrolltype = 2;
	else if (strLowerMessage.substr(intMsgCnt, 2) == "re")Rrolltype = 3;
	intMsgCnt += 2;
	string strSkillName;
	bool addmode = 0;
	bool minmode = 0;
	int dicetype = 1;
	bool dicehide = 0;
	int dicechain = 0;
	int DiceNumber = 2;
	bool diceinsert = 0;
	while (intMsgCnt != strLowerMessage.length() && strLowerMessage[intMsgCnt] == 's' || strLowerMessage[intMsgCnt] ==
		'h' || strLowerMessage[intMsgCnt] == 'p' || strLowerMessage[intMsgCnt] == 'm' || strLowerMessage[intMsgCnt] == 'k' || strLowerMessage[intMsgCnt] == 'b' || strLowerMessage[intMsgCnt] == 't')
	{
		if (strLowerMessage[intMsgCnt] == 'h')dicehide = 1;
		else if (strLowerMessage[intMsgCnt] == 's')dicechain = 1;
		else if (strLowerMessage[intMsgCnt] == 't')dicechain = 2;
		else if (strLowerMessage[intMsgCnt] == 'k'&&dice_msg.msg_type != Dice::MsgType::Private)dicechain = 3;
		else if (strLowerMessage[intMsgCnt] == 'p')dicetype = 2;
		else if (strLowerMessage[intMsgCnt] == 'b') dicetype = 3;
		else if (strLowerMessage[intMsgCnt] == 'm') diceinsert = 1;
		intMsgCnt++;
	}
	while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && dicetype != 1)
	{
		DiceNumber = int(strLowerMessage[intMsgCnt] - '0');
		intMsgCnt++;
	}
	while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
	{
		intMsgCnt++;
	}
	string strReply;
	if (dicechain == 1)
	{
		int Circlytime = 0;
		strReply = strNickName + "���������춨��";
		int Tdicetype, TDiceNumber;
		bool Tdicehide;
		string Temskill;
		string strSkillVal;
		while (intMsgCnt != strLowerMessage.length())
		{
			Tdicetype = dicetype;
			Tdicehide = dicehide;
			TDiceNumber = DiceNumber;
			if (strLowerMessage[intMsgCnt] == '#')intMsgCnt++;
			else
			{
				addmode = 0;
				strSkillName.clear();
				strSkillVal.clear();
				while (intMsgCnt != strLowerMessage.length() && strLowerMessage[intMsgCnt] == 'p' || strLowerMessage[intMsgCnt] == 'b')
				{
					if (strLowerMessage[intMsgCnt] == 'p')dicetype = 2;
					else if (strLowerMessage[intMsgCnt] == 'b') dicetype = 3;
					intMsgCnt++;
				}
				while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && dicetype != 1)
				{
					DiceNumber = int(strLowerMessage[intMsgCnt] - '0');
					intMsgCnt++;
				}
				while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
					isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt] !=
					':' && strLowerMessage[intMsgCnt] != '#'&& strLowerMessage[intMsgCnt] != '+'&& strLowerMessage[intMsgCnt] != '-')
				{
					strSkillName += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
				Temskill = strSkillName;
				if (strSkillName.empty() && Temskill.empty() != 1)strSkillName = Temskill;
				if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
				while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] ==
					':')intMsgCnt++;
				while (strLowerMessage[intMsgCnt] == '-' || strLowerMessage[intMsgCnt] == '+')
				{
					addmode = 1;
					if (strLowerMessage[intMsgCnt] == '-')minmode = 1;
					intMsgCnt++;
				}
				while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
				{
					strSkillVal += strLowerMessage[intMsgCnt];
					intMsgCnt++;
				}
			}
			while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
			{
				intMsgCnt++;
			}
			int intSkillVal = 0;
			if ( Cname == "default" && (addmode||strSkillVal.empty()))
			{
				if (CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].count(strSkillName))
				{
					intSkillVal = CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName];
				}
				else if (SkillDefaultVal.count(strSkillName))
				{
					intSkillVal = SkillDefaultVal[strSkillName];
				}
				else if(strSkillVal.empty())
				{
					strReply += "\nδ�ҵ�" + strSkillName + "����ֵ";
					strSkillName.clear();
					strSkillVal.clear();
					continue;
				}
				if (addmode && intSkillVal + stoi(strSkillVal) >= 999)
				{
					dice_msg.Reply(GlobalMsg["strDiceInsertValTooBig"]);
					return;
				}
				else if (addmode && minmode && intSkillVal + stoi(strSkillVal) < 0)
				{
					dice_msg.Reply(GlobalMsg["strDiceInsertValTooSmall"]);
					return;
				}
				else if (addmode && minmode == 0)intSkillVal += stoi(strSkillVal);
				else if (addmode && minmode)intSkillVal -= stoi(strSkillVal);
			}
			else if (addmode||strSkillVal.empty())
			{
				const auto range = Clist.equal_range(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id));
				multimap<SourceType, Listcon>::iterator Cfinder1 = range.first;
				map<string, PropType>::iterator finder2;
				bool found = 1;
				for (; Cfinder1 != range.second;)
				{
					finder2 = Cfinder1->second.begin();
					if (finder2->first == Cname && finder2->second.count(strSkillName))
					{
						intSkillVal = finder2->second[strSkillName];
						found = 0;
					}
					if (++Cfinder1 == Clist.end())break;
				}
				if (found && SkillDefaultVal.count(strSkillName))
				{
					intSkillVal = SkillDefaultVal[strSkillName];
				}
				else if (found && SkillDefaultVal.count(strSkillName) == 0&& strSkillVal.empty())
				{
					strReply += "\nδ�ҵ�" + strSkillName + "����ֵ";
					strSkillName.clear();
					strSkillVal.clear();
					continue;
				}
				if (addmode && intSkillVal + stoi(strSkillVal) >= 999)
				{
					dice_msg.Reply(GlobalMsg["strDiceInsertValTooBig"]);
					return;
				}
				else if (addmode && minmode && intSkillVal + stoi(strSkillVal) < 0)
				{
					dice_msg.Reply(GlobalMsg["strDiceInsertValTooSmall"]);
					return;
				}
				else if (addmode && minmode == 0)intSkillVal += stoi(strSkillVal);
				else if (addmode && minmode)intSkillVal -= stoi(strSkillVal);
			}
			else if (strSkillVal.length() > 3)
			{
				strReply += strSkillName + "\n�ļ�����ֵ����";
				strSkillName.clear();
				strSkillVal.clear();
				continue;
			}
			else
			{
				intSkillVal = stoi(strSkillVal);
			}
			int a[10];
			int cosdice = 0;
			int intD100Res;
			int sindice = RandomGenerator::Randint(0, 9);
			if (dicetype != 1)
			{
				if (sindice == 0)for (int i = 0; i < DiceNumber; i++) a[i] = RandomGenerator::Randint(1, 10);
				else for (int i = 0; i < DiceNumber; i++) a[i] = RandomGenerator::Randint(0, 9);
				if (dicetype == 2)
				{
					cosdice = 0;
					for (int i = 0; i < DiceNumber; i++)if (cosdice < a[i])cosdice = a[i];
				}
				else if (dicetype == 3)
				{
					cosdice = 10;
					for (int i = 0; i < DiceNumber; i++)if (cosdice > a[i])cosdice = a[i];
				}
				intD100Res = cosdice * 10 + sindice;
			}
			else intD100Res = RandomGenerator::Randint(1, 100);
			if (dicehide)strReply += "\n#" + strSkillName + "�춨������ɾ������";
			else
			{
				strReply += "\n#" + strSkillName + "�춨�Ľ���ǣ���\n: D100=" + to_string(intD100Res) + "/" + to_string(intSkillVal) + "";
				if (dicetype == 2)strReply += "\n[�ͷ���]:";
				else if (dicetype == 3)strReply += "\n[������]:";
				if (dicetype != 1)for (int i = 0; i < DiceNumber; i++)strReply += to_string(a[i]) + " ";
			}
			int checkres = Checker(intD100Res, intSkillVal, Rrolltype);//���м��ܼ춨
			strReply += "\n" + ResultGet(checkres);
			if (checkres == 1)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
			else if (checkres == 6)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]++;
			s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3]++;
			if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] == NULL)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] = intD100Res;
			else s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] += intD100Res;
			if (Circlytime > 3)
			{
				strReply += "\n����̫�������������ҾͲ���������";
				break;
			}
			dicetype = Tdicetype;
			dicehide = Tdicehide;
			DiceNumber = TDiceNumber;
			strSkillVal = to_string(intSkillVal);
			Circlytime++;
		}
		dice_msg.Reply(strReply);
	}
	else
	{
		while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && dicetype != 1)
		{
			DiceNumber = int(strLowerMessage[intMsgCnt] - '0');
			intMsgCnt++;
		}

		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))intMsgCnt++;
		while (intMsgCnt != strLowerMessage.length() && !isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && !
			isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) && strLowerMessage[intMsgCnt] != '=' && strLowerMessage[intMsgCnt] !=
			':'&& strLowerMessage[intMsgCnt] !='+'&& strLowerMessage[intMsgCnt] != '-')
		{
			strSkillName += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
		if (SkillNameReplace.count(strSkillName))strSkillName = SkillNameReplace[strSkillName];
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])) || strLowerMessage[intMsgCnt] == '=' || strLowerMessage[intMsgCnt] ==
			':')intMsgCnt++;
		string strSkillVal;
		while (strLowerMessage[intMsgCnt] == '-'|| strLowerMessage[intMsgCnt] == '+')
		{
			addmode = 1;
			if(strLowerMessage[intMsgCnt] == '-')minmode = 1;
			intMsgCnt++;
		}
		while (isdigit(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		{
			strSkillVal += strLowerMessage[intMsgCnt];
			intMsgCnt++;
		}
		while (isspace(static_cast<unsigned char>(strLowerMessage[intMsgCnt])))
		{
			intMsgCnt++;
		}
		string strReason = strLowerMessage.substr(intMsgCnt);
		int intSkillVal = 0;
		if (Cname == "default" && (addmode || strSkillVal.empty()))
		{
			if (CharacterProp.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) && CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].count(strSkillName))
			{
				intSkillVal = CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName];
			}
			else if (SkillDefaultVal.count(strSkillName))
			{
				intSkillVal = SkillDefaultVal[strSkillName];
			}
			else if (strSkillVal.empty())
			{
				dice_msg.Reply(GlobalMsg["strUnknownPropErr"]);
				return;
			}
			if (addmode && intSkillVal + stoi(strSkillVal) >= 999)
			{
				dice_msg.Reply(GlobalMsg["strDiceInsertValTooBig"]);
				return;
			}
			else if (addmode && minmode && intSkillVal + stoi(strSkillVal) < 0)
			{
				dice_msg.Reply(GlobalMsg["strDiceInsertValTooSmall"]);
				return;
			}
			else if (addmode && minmode == 0)intSkillVal += stoi(strSkillVal);
			else if (addmode && minmode)intSkillVal -= stoi(strSkillVal);
		}
		else if (addmode || strSkillVal.empty())
		{
			bool found = ClistFounder(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id),Cname);
			if (found == 0)
			{
				dice_msg.Reply(GlobalMsg["strCEmpty"]);
				return;
			}
			else if (F2Fsk->second.count(strSkillName))
			{
				intSkillVal = F2Fsk->second[strSkillName];
			}
			else if (SkillDefaultVal.count(strSkillName))
			{
				intSkillVal = SkillDefaultVal[strSkillName];
			}
			else if (SkillDefaultVal.count(strSkillName) == 0)
			{
				dice_msg.Reply(GlobalMsg["strUnknownPropErr"]);
				return;
			}
			if (addmode && intSkillVal + stoi(strSkillVal) >= 999)
			{
				dice_msg.Reply(GlobalMsg["strDiceInsertValTooBig"]);
				return;
			}
			else if (addmode && minmode && intSkillVal + stoi(strSkillVal) < 0)
			{
				dice_msg.Reply(GlobalMsg["strDiceInsertValTooSmall"]);
				return;
			}
			else if(addmode&&minmode==0)intSkillVal += stoi(strSkillVal);
			else if (addmode && minmode)intSkillVal -= stoi(strSkillVal);

		}
		else if (strSkillVal.length() > 3)
		{
			dice_msg.Reply(GlobalMsg["strPropErr"]);
			return;
		}
		else
		{
			intSkillVal = stoi(strSkillVal);
		}
		//�����Զ�¼��
		if (diceinsert == 1 && Cname != "default")
		{
			ClistFounder(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id),Cname);
			F2Fsk->second[strSkillName] = intSkillVal;
			strReply += strSkillName + "�ļ���ֵ" + to_string(intSkillVal) + "�Ѿ��Զ�¼��\n";
		}
		else if (diceinsert == 1)
		{
			strReply += strSkillName + "�ļ���ֵ" + to_string(intSkillVal) + "�Ѿ��Զ�¼��\n";
			CharacterProp[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][strSkillName] = intSkillVal;
		}
		int a[10];
		int cosdice = 0;
		int intD100Res;
		int sindice = RandomGenerator::Randint(0, 9);
		if (dicetype != 1)
		{
			if (sindice == 0)for (int i = 0; i < DiceNumber; i++) a[i] = RandomGenerator::Randint(1, 10);
			else for (int i = 0; i < DiceNumber; i++) a[i] = RandomGenerator::Randint(0, 9);
			if (dicetype == 2)
			{
				cosdice = 0;
				for (int i = 0; i < DiceNumber; i++)if (cosdice < a[i])cosdice = a[i];
			}
			else if (dicetype == 3)
			{
				cosdice = 10;
				for (int i = 0; i < DiceNumber; i++)if (cosdice > a[i])cosdice = a[i];
			}
			intD100Res = cosdice * 10 + sindice;
		}
		else intD100Res = RandomGenerator::Randint(1, 100);
		if (dicechain == 2)
		{
			if (dice_msg.msg_type == Dice::MsgType::Private&&KpGroup.count(SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)) == 0)
			{
				dice_msg.Reply("�㵱ǰû��kpȺ����Ҫ�ҷ�������ȥѽˤ");
				return;
			}
			TypeNumber st;
			if (dice_msg.msg_type == Dice::MsgType::Private)st = KpGroup[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)];
			else st = { dice_msg.group_id,dice_msg.msg_type };
			strReply = strNickName + "������" + strSkillName + "�Կ����춨���Ϊ������ɾ����";
			GroupDuel[st][0] = intD100Res;
			GroupDuel[st][1] = intSkillVal;
			GroupDuel[st][3] = 1;
			if (dicehide)
				GroupDuel[st][4] = 1;
			else
				GroupDuel[st][4] = 0;
			GroupDuel[st][2] = Checker(intD100Res, intSkillVal, Rrolltype);//���м��ܼ춨
			if (GroupDuel[st][2] == 1)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
			else if (GroupDuel[st][2] == 6)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]++;
		}
		else if (dicechain == 3  &&  dice_msg.msg_type != Dice::MsgType::Private)
		{
			if (GroupDuel[{dice_msg.group_id, dice_msg.msg_type}][3] == 0)
			{
				dice_msg.Reply(to_string(GroupDuel[{dice_msg.group_id, dice_msg.msg_type}][3]));
				dice_msg.Reply("��ǰȺ/��������û�����ڽ����еĶԿ�����ʹ��.rat/.rath + ���ܼ춨���жԿ�����");
				return;
			}
			strReply = strNickName + "���жԿ���" + to_string(intD100Res) + "/" + to_string(intSkillVal) + "\n";
			//GroupDuel[{dice_msg.group_id,dice_msg.msg_type}];
			int Data1[3];
			Data1[0] = intD100Res;
			Data1[1] = intSkillVal;
			if (Rrolltype == 1)
			{
				if (intD100Res <= 5 && intD100Res <= intSkillVal)
				{
					Data1[2] = 1;
					strReply += "�춨��ɹ���";
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
				}
				else if (intD100Res <= intSkillVal / 5)
				{
					strReply += "�춨���޳ɹ���";
					Data1[2] = 2;
				}
				else if (intD100Res <= intSkillVal / 2)
				{
					Data1[2] = 3;
					strReply += "�춨���ѳɹ���";
				}
				else if (intD100Res <= intSkillVal)
				{
					Data1[2] = 4;
					strReply += "�춨�ɹ���";
				}
				else if (intD100Res <= 95)
				{
					Data1[2] = 5;
					strReply += "�춨ʧ�ܣ�";
				}
				else
				{
					Data1[2] = 6;
					strReply += "�춨��ʧ�ܣ�";
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]++;
				}
			}
			else if (Rrolltype == 2)
			{
				if (intD100Res == 1 && intSkillVal <= 50 && intD100Res <= intSkillVal)
				{
					Data1[2] = 1;
					strReply += "�춨��ɹ���";
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
				}
				else if (intD100Res <= 5 && intSkillVal >= 50 && intD100Res <= intSkillVal)
				{
					Data1[2] = 1;
					strReply += "�춨��ɹ���";
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
				}
				else if (intD100Res <= intSkillVal / 5)
				{
					strReply += "�춨���޳ɹ���";
					Data1[2] = 2;
				}
				else if (intD100Res <= intSkillVal / 2)
				{
					strReply += "�춨���ѳɹ���";
					Data1[2] = 3;
				}
				else if (intD100Res <= intSkillVal)
				{
					strReply += "�춨�ɹ���";
					Data1[2] = 4;
				}
				else if (intD100Res <= 95 || (intSkillVal >= 50 && intD100Res != 100))
				{
					strReply += "�춨ʧ�ܣ�";
					Data1[2] = 5;
				}
				else
				{
					strReply += "�춨��ʧ�ܣ�";
					Data1[2] = 6;
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]++;
				}
			}
			else
			{
				if (intD100Res == 1)
				{
					strReply += "�춨��ɹ���";
					Data1[2] = 1;
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
				}
				else if (intD100Res <= intSkillVal / 5)
				{
					strReply += "�춨���޳ɹ���";
					Data1[2] = 2;
				}
				else if (intD100Res <= intSkillVal / 2)
				{
					strReply += "�춨���ѳɹ���";
					Data1[2] = 3;
				}
				else if (intD100Res <= intSkillVal)
				{
					strReply += "�춨�ɹ���";
					Data1[2] = 4;
				}
				else if (intD100Res <= 95 || (intSkillVal >= 50 && intD100Res != 100))
				{
					strReply += "�춨ʧ�ܣ�";
					Data1[2] = 5;
				}
				else
				{
					strReply += "�춨��ʧ�ܣ�";
					Data1[2] = 6;
					s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]++;
				}
			}
			if (Data1[2] < GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][2])
			{
				strReply += "�Կ��ɹ�";
				if (GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][4] == 0)strReply += ",�춨�ȼ���" + to_string(GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][2] - Data1[2]) + "��";
			}
			else if (Data1[2] > GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][2])
			{
				strReply += "�Կ�ʧ��";
				if (GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][4] == 0)strReply += "���춨�ȼ���" + to_string(Data1[2] - GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][2]) + "��";
			}
			else if (Data1[0] < GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][0])
			{
				strReply += "�Կ��ɹ�";
				if (GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][4] == 0)strReply += "���춨�ȼ�ƽ���������ʤ��";
			}
			else if (Data1[0] > GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][0])
			{
				strReply += "�Կ�ʧ��";
				if (GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][4] == 0)strReply += "���춨�ȼ�ƽ���������ʧ��";
			}
			else if (Data1[1] < GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][1])
			{
				strReply += "�Կ��ɹ�";
				if (GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][4] == 0)strReply += "���춨�ȼ�ƽ������ƽ�����ܵȼ�ʤ��";
			}
			else if (Data1[1] > GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][1])
			{
				strReply += "�Կ�ʧ��";
				if (GroupDuel[{dice_msg.group_id,dice_msg.msg_type}][4] == 0)strReply += "���춨�ȼ�ƽ������ƽ�����ܵȼ�ʤ��";
			}
			else strReply += "Ŷ����ƽ��";
		}
		if (dicechain == 0)
		{
			if (dicehide) strReply += strNickName + "��" + strSkillName + "�춨������ɾ������";
			else
			{
				strReply += strNickName + "���Ž�����һ��" + strSkillName + "�춨������ǣ�: D100=" + to_string(intD100Res) + "/" + to_string(intSkillVal) + "";
				if (dicetype == 2)strReply += "\n[�ͷ���]:";
				else if (dicetype == 3)strReply += "\n[������]:";
				if (dicetype != 1)for (int i = 0; i < DiceNumber; i++)strReply += to_string(a[i]) + " ";
			}
			int checkres = Checker(intD100Res,intSkillVal, Rrolltype);//���м��ܼ춨
			strReply += "\n" + RandomReply::getReply("SkiCheck"+to_string(checkres));
			if(checkres==1)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][1]++;
			else if(checkres==6)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][2]++;
		}
		s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][3]++;
		if (s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] == NULL)s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] = intD100Res;
		else s_Count[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)][4] += intD100Res;
		if (!strReason.empty())
		{
			strReply = "����" + strReason + " " + strReply;
		}
		if (dicechain == 2)
		{
			TypeNumber st;
			if (dice_msg.msg_type == Dice::MsgType::Private)st = KpGroup[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)];
			else st = { dice_msg.group_id,dice_msg.msg_type };
			string Dreply = "��������" + to_string(intD100Res) + "/" + to_string(intSkillVal) + "\n���Ϊ��" + ResultGet(GroupDuel[st][2]);
			AddMsgToQueue(Dice::DiceMsg(Dreply, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
		}
		if (dicechain == 2 && dice_msg.msg_type == Dice::MsgType::Private)
			AddMsgToQueue(Dice::DiceMsg(strReply,KpGroup[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].first, dice_msg.qq_id, KpGroup[SourceType(dice_msg.qq_id, dice_msg.msg_type, dice_msg.group_id)].second));
		else
		dice_msg.Reply(strReply);
	}
	}
		else if (strLowerMessage[intMsgCnt] == 'r')
		{
			int intSkillVal = -1;
			string strSkillVal = "";
			intMsgCnt += 1;
			bool boolDetail = true, isHidden = false;
			if (dice_msg.msg[intMsgCnt] == 's')
			{
				boolDetail = false;
				intMsgCnt++;
			}
			if (strLowerMessage[intMsgCnt] == 'h')
			{
				isHidden = true;
				intMsgCnt++;
			}
			while (isspace(static_cast<unsigned char>(dice_msg.msg[intMsgCnt])))
				intMsgCnt++;
			string strMainDice;
			string strReason;
			bool tmpContainD = false;
			int intTmpMsgCnt;
			for (intTmpMsgCnt = intMsgCnt; intTmpMsgCnt != dice_msg.msg.length() && dice_msg.msg[intTmpMsgCnt] != ' ';
				intTmpMsgCnt++)
			{
				if (strLowerMessage[intTmpMsgCnt] == 'd' || strLowerMessage[intTmpMsgCnt] == 'p' || strLowerMessage[intTmpMsgCnt] == 'b' || strLowerMessage[intTmpMsgCnt] == '#' || strLowerMessage[intTmpMsgCnt] == 'f' || strLowerMessage[intTmpMsgCnt] == 'a')
					tmpContainD = true;
				if (!isdigit(static_cast<unsigned char>(strLowerMessage[intTmpMsgCnt])) && strLowerMessage[intTmpMsgCnt] != 'd' && strLowerMessage[intTmpMsgCnt] != 'k' && strLowerMessage[intTmpMsgCnt] != 'p' && strLowerMessage[intTmpMsgCnt] != 'b' &&
					strLowerMessage[intTmpMsgCnt] != 'f' && strLowerMessage[intTmpMsgCnt] != '+' && strLowerMessage[intTmpMsgCnt] != '-' && strLowerMessage[intTmpMsgCnt] != '#' && strLowerMessage[intTmpMsgCnt] != 'a' && strLowerMessage[intTmpMsgCnt] != 'x' && strLowerMessage[intTmpMsgCnt] != '*')
				{
					break;
				}
			}
			if (tmpContainD)
			{
				strMainDice = strLowerMessage.substr(intMsgCnt, intTmpMsgCnt - intMsgCnt);
				while (isspace(static_cast<unsigned char>(dice_msg.msg[intTmpMsgCnt])))
					intTmpMsgCnt++;
				strReason = dice_msg.msg.substr(intTmpMsgCnt);
				if (split(strLowerMessage, " ").size() > 1)
				{
					if (AllisNum(split(strLowerMessage, " ")[1]))
					{
						string tmpSkillVal = split(strLowerMessage, " ")[1];
						trim(tmpSkillVal);
						strSkillVal = tmpSkillVal;
						intSkillVal = getNumber(tmpSkillVal.c_str());
					}
				}
			}
			else
				strReason = dice_msg.msg.substr(intMsgCnt);

			int intTurnCnt = 1;
			if (strMainDice.find("#") != string::npos)
			{
				string strTurnCnt = strMainDice.substr(0, strMainDice.find("#"));
				if (strTurnCnt.empty())
					strTurnCnt = "1";
				strMainDice = strMainDice.substr(strMainDice.find("#") + 1);
				RD rdTurnCnt(strTurnCnt, dice_msg.qq_id);
				const int intRdTurnCntRes = rdTurnCnt.Roll();
				if (intRdTurnCntRes == Value_Err)
				{
					dice_msg.Reply(GlobalMsg["strValueErr"]);
					return;
				}
				if (intRdTurnCntRes == Input_Err)
				{
					dice_msg.Reply(GlobalMsg["strInputErr"]);
					return;
				}
				if (intRdTurnCntRes == ZeroDice_Err)
				{
					dice_msg.Reply(GlobalMsg["strZeroDiceErr"]);
					return;
				}
				if (intRdTurnCntRes == ZeroType_Err)
				{
					dice_msg.Reply(GlobalMsg["strZeroTypeErr"]);
					return;
				}
				if (intRdTurnCntRes == DiceTooBig_Err)
				{
					dice_msg.Reply(GlobalMsg["strDiceTooBigErr"]);
					return;
				}
				if (intRdTurnCntRes == TypeTooBig_Err)
				{
					dice_msg.Reply(GlobalMsg["strTypeTooBigErr"]);
					return;
				}
				if (intRdTurnCntRes == AddDiceVal_Err)
				{
					dice_msg.Reply(GlobalMsg["strAddDiceValErr"]);
					return;
				}
				if (intRdTurnCntRes != 0)
				{
					dice_msg.Reply(GlobalMsg["strUnknownErr"]);
					return;
				}
				if (rdTurnCnt.intTotal > 10)
				{
					dice_msg.Reply(GlobalMsg["strRollTimeExceeded"]);
					return;
				}
				if (rdTurnCnt.intTotal <= 0)
				{
					dice_msg.Reply(GlobalMsg["strRollTimeErr"]);
					return;
				}
				intTurnCnt = rdTurnCnt.intTotal;
				if (strTurnCnt.find("d") != string::npos)
				{
					string strTurnNotice = strNickName + "����������: " + rdTurnCnt.FormShortString() + "��";
					if (!isHidden)
					{
						Log->record(dice_msg.group_id, "", strTurnNotice);
						dice_msg.Reply(strTurnNotice);
					}
					else
					{
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							Log->record(dice_msg.group_id, "", strTurnNotice);
							strTurnNotice = "��Ⱥ\"" + getGroupList()[dice_msg.group_id] + "\"�� " + strTurnNotice;
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							Log->record(dice_msg.group_id, "", strTurnNotice);
							strTurnNotice = "�ڶ��������� " + strTurnNotice;
						}
						AddMsgToQueue(Dice::DiceMsg(strTurnNotice, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
						pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							range = ObserveGroup.equal_range(dice_msg.group_id);
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							range = ObserveDiscuss.equal_range(dice_msg.group_id);
						}
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != dice_msg.qq_id)
							{
								AddMsgToQueue(Dice::DiceMsg(strTurnNotice, 0LL, it->second, Dice::MsgType::Private));
							}
						}
					}
				}
			}
			RD rdMainDice(strMainDice, dice_msg.qq_id);

			const int intFirstTimeRes = rdMainDice.Roll();
			if (intFirstTimeRes == Value_Err)
			{
				dice_msg.Reply(GlobalMsg["strValueErr"]);
				return;
			}
			if (intFirstTimeRes == Input_Err)
			{
				dice_msg.Reply(GlobalMsg["strInputErr"]);
				return;
			}
			if (intFirstTimeRes == ZeroDice_Err)
			{
				dice_msg.Reply(GlobalMsg["strZeroDiceErr"]);
				return;
			}
			if (intFirstTimeRes == ZeroType_Err)
			{
				dice_msg.Reply(GlobalMsg["strZeroTypeErr"]);
				return;
			}
			if (intFirstTimeRes == DiceTooBig_Err)
			{
				dice_msg.Reply(GlobalMsg["strDiceTooBigErr"]);
				return;
			}
			if (intFirstTimeRes == TypeTooBig_Err)
			{
				dice_msg.Reply(GlobalMsg["strTypeTooBigErr"]);
				return;
			}
			if (intFirstTimeRes == AddDiceVal_Err)
			{
				dice_msg.Reply(GlobalMsg["strAddDiceValErr"]);
				return;
			}
			if (intFirstTimeRes != 0)
			{
				dice_msg.Reply(GlobalMsg["strUnknownErr"]);
				return;
			}
			if (!boolDetail && intTurnCnt != 1)
			{
				string strAns = strNickName + "������: " + to_string(intTurnCnt) + "��" + rdMainDice.strDice + ": { ";
				if (!strReason.empty())
					strAns.insert(0, "����" + strReason + " ");
				vector<int> vintExVal;
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					strAns += to_string(rdMainDice.intTotal);
					if (intTurnCnt != 0)
						strAns += ",";
					if ((rdMainDice.strDice == "D100" || rdMainDice.strDice == "1D100") && (rdMainDice.intTotal <= 5 ||
						rdMainDice.intTotal >= 96))
						vintExVal.push_back(rdMainDice.intTotal);
				}
				strAns += " }";
				if (!vintExVal.empty())
				{
					strAns += ",��ֵ: ";
					for (auto it = vintExVal.cbegin(); it != vintExVal.cend(); ++it)
					{
						strAns += to_string(*it);
						if (it != vintExVal.cend() - 1)
							strAns += ",";
					}
				}
				if (!isHidden)
				{
					dice_msg.Reply(strAns);
				}
				else
				{
					if (dice_msg.msg_type == Dice::MsgType::Group)
					{
						strAns = "��Ⱥ\"" + getGroupList()[dice_msg.group_id] + "\"�� " + strAns;
					}
					else if (dice_msg.msg_type == Dice::MsgType::Discuss)
					{
						strAns = "�ڶ��������� " + strAns;
					}
					AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
					pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
					if (dice_msg.msg_type == Dice::MsgType::Group)
					{
						range = ObserveGroup.equal_range(dice_msg.group_id);
					}
					else if (dice_msg.msg_type == Dice::MsgType::Discuss)
					{
						range = ObserveDiscuss.equal_range(dice_msg.group_id);
					}
					for (auto it = range.first; it != range.second; ++it)
					{
						if (it->second != dice_msg.qq_id)
						{
							AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, it->second, Dice::MsgType::Private));
						}
					}
				}
			}
			else
			{
				while (intTurnCnt--)
				{
					// �˴�����ֵ����
					// ReSharper disable once CppExpressionWithoutSideEffects
					rdMainDice.Roll();
					string strAns = strNickName + "������: " + (boolDetail ? rdMainDice.FormCompleteString() : rdMainDice.FormShortString());

					if (split(strReason, " ").size() >= 2)
					{
						strReason = split(strReason, " ")[1];
					}
					if (AllisNum(strReason))
					{
						strReason = "";
					}
					if (!strReason.empty())
						strAns.insert(0, "����" + strReason + " ");
					if (!isHidden)
					{
						if (intSkillVal != -1)
						{
							strAns += "/" + std::to_string(intSkillVal) + " ";
							int ResultValue = stoi(split(rdMainDice.FormShortString(), "=")[1]);

							if (ResultValue <= 5 && ResultValue <= intSkillVal)
								strAns += GlobalMsg["strCriticalSuccess"];
							else if (ResultValue > 95)
								strAns += GlobalMsg["strFumble"];
							else if (ResultValue <= intSkillVal / 5)
								strAns += GlobalMsg["strExtremeSuccess"];
							else if (ResultValue <= intSkillVal / 2)
								strAns += GlobalMsg["strHardSuccess"];
							else if (ResultValue <= intSkillVal)
								strAns += GlobalMsg["strSuccess"];
							else
								strAns += GlobalMsg["strFailure"];
							Log->record(dice_msg.group_id, "", strAns);

							dice_msg.Reply(strAns);
						}
						else
						{
							Log->record(dice_msg.group_id, "", strAns);
							dice_msg.Reply(strAns);
						}
					}
					else
					{
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							Log->record(dice_msg.group_id, "", strAns);
							strAns = "��Ⱥ\"" + getGroupList()[dice_msg.group_id] + "\"�� " + strAns;
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							Log->record(dice_msg.group_id, "", strAns);
							strAns = "�ڶ��������� " + strAns;
						}
						AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, dice_msg.qq_id, Dice::MsgType::Private));
						pair<multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> range;
						if (dice_msg.msg_type == Dice::MsgType::Group)
						{
							range = ObserveGroup.equal_range(dice_msg.group_id);
						}
						else if (dice_msg.msg_type == Dice::MsgType::Discuss)
						{
							range = ObserveDiscuss.equal_range(dice_msg.group_id);
						}
						for (auto it = range.first; it != range.second; ++it)
						{
							if (it->second != dice_msg.qq_id)
							{
								AddMsgToQueue(Dice::DiceMsg(strAns, 0LL, it->second, Dice::MsgType::Private));
							}
						}
					}
				}
			}
			if (isHidden)
			{
				const string strReply = strNickName + "������һ�ΰ���";
				Log->record(dice_msg.group_id, "", strReply);
				dice_msg.Reply(strReply);
			}
		}
		else
		{
			block_msg = false;
		}
	}
	void EventHandler::HandleGroupMemberIncreaseEvent(long long beingOperateQQ, long long fromGroup)
	{

		if (beingOperateQQ != getLoginQQ() && WelcomeMsg.count(fromGroup))
		{
			string strReply = WelcomeMsg[fromGroup];
			while (strReply.find("{@}") != string::npos)
			{
				strReply.replace(strReply.find("{@}"), 3, "[CQ:at,qq=" + to_string(beingOperateQQ) + "]");
			}
			while (strReply.find("{nick}") != string::npos)
			{
				strReply.replace(strReply.find("{nick}"), 6, getStrangerInfo(beingOperateQQ).nick);
			}
			while (strReply.find("{age}") != string::npos)
			{
				strReply.replace(strReply.find("{age}"), 5, to_string(getStrangerInfo(beingOperateQQ).age));
			}
			while (strReply.find("{sex}") != string::npos)
			{
				strReply.replace(strReply.find("{sex}"), 5,
					getStrangerInfo(beingOperateQQ).sex == 0
					? "��"
					: getStrangerInfo(beingOperateQQ).sex == 1
					? "Ů"
					: "δ֪");
			}
			while (strReply.find("{qq}") != string::npos)
			{
				strReply.replace(strReply.find("{qq}"), 4, to_string(beingOperateQQ));
			}
			AddMsgToQueue(Dice::DiceMsg(std::move(strReply), fromGroup, fromGroup, Dice::MsgType::Group));
		}
	}
	void EventHandler::HandleDisableEvent()
	{
		Enabled = false;
		ilInitList.reset();
		Name.reset();
		ofstream ofstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); ++it)
		{
			ofstreamDisabledGroup << *it << std::endl;
		}
		ofstreamDisabledGroup.close();

		ofstream ofstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); ++it)
		{
			ofstreamDisabledDiscuss << *it << std::endl;
		}
		ofstreamDisabledDiscuss.close();
		ofstream ofstreamDisabledJRRPGroup(strFileLoc + "DisabledJRRPGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledJRRPGroup.begin(); it != DisabledJRRPGroup.end(); ++it)
		{
			ofstreamDisabledJRRPGroup << *it << std::endl;
		}
		ofstreamDisabledJRRPGroup.close();

		ofstream ofstreamUsingname(strFileLoc + "Usingname.RDconf", ios::out | ios::trunc);
		for (auto it = Usingname.begin(); it != Usingname.end(); ++it)
		{
			ofstreamUsingname << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " " << it->second << std::endl;
		}
		ofstreamUsingname.close();

		ofstream ofstreamClist(strFileLoc + "Clist.RDconf", ios::out | ios::trunc);
		for (auto it = Clist.begin(); it != Clist.end(); ++it)
		{
			for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
			{
				for (auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); ++it2)
				{
					ofstreamClist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
						<< it1->first << " " << it2->first << " " << it2->second << std::endl;
				}
			}
		}
		ofstreamClist.close();

		ofstream ofstreamDuellist(strFileLoc + "Duellist.RDconf", ios::out | ios::trunc);
		for (auto it = Duellist.begin(); it != Duellist.end(); ++it)
		{
			ofstreamDuellist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it->second << std::endl;
		}
		ofstreamDuellist.close();

		ofstream ofstreams_Count(strFileLoc + "s_Count.RDconf", ios::out | ios::trunc);
		for (auto it = s_Count.begin(); it != s_Count.end(); ++it)
		{
			ofstreams_Count << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it->second[1] << " "
				<< it->second[2] << " "
				<< it->second[3] << " "
				<< it->second[4] << " "
				<< it->second[5] << " "
				<< it->second[6] << " "
				<< it->second[7] << " "
				<< it->second[8] << " " << std::endl;
		}
		ofstreams_Count.close();

		ofstream ofstreamDisabledJRRPDiscuss(strFileLoc + "DisabledJRRPDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledJRRPDiscuss.begin(); it != DisabledJRRPDiscuss.end(); ++it)
		{
			ofstreamDisabledJRRPDiscuss << *it << std::endl;
		}
		ofstreamDisabledJRRPDiscuss.close();

		ofstream ofstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf", ios::in | ios::trunc);
		for (auto it = DisabledHELPGroup.begin(); it != DisabledHELPGroup.end(); ++it)
		{
			ofstreamDisabledHELPGroup << *it << std::endl;
		}
		ofstreamDisabledHELPGroup.close();

		ofstream ofstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf", ios::in | ios::trunc);
		for (auto it = DisabledHELPDiscuss.begin(); it != DisabledHELPDiscuss.end(); ++it)
		{
			ofstreamDisabledHELPDiscuss << *it << std::endl;
		}
		ofstreamDisabledHELPDiscuss.close();

		ofstream ofstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledOBGroup.begin(); it != DisabledOBGroup.end(); ++it)
		{
			ofstreamDisabledOBGroup << *it << std::endl;
		}
		ofstreamDisabledOBGroup.close();

		ofstream ofstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledOBDiscuss.begin(); it != DisabledOBDiscuss.end(); ++it)
		{
			ofstreamDisabledOBDiscuss << *it << std::endl;
		}
		ofstreamDisabledOBDiscuss.close();

		ofstream ofstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf", ios::out | ios::trunc);
		for (auto it = ObserveGroup.begin(); it != ObserveGroup.end(); ++it)
		{
			ofstreamObserveGroup << it->first << " " << it->second << std::endl;
		}
		ofstreamObserveGroup.close();

		ofstream ofstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = ObserveDiscuss.begin(); it != ObserveDiscuss.end(); ++it)
		{
			ofstreamObserveDiscuss << it->first << " " << it->second << std::endl;
		}
		ofstreamObserveDiscuss.close();
		ofstream ofstreamCharacterProp(strFileLoc + "CharacterProp.RDconf", ios::out | ios::trunc);
		for (auto it = CharacterProp.begin(); it != CharacterProp.end(); ++it)
		{
			for (auto it1 = it->second.cbegin(); it1 != it->second.cend(); ++it1)
			{
				ofstreamCharacterProp << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
					<< it1->first << " " << it1->second << std::endl;
			}
		}
		ofstreamCharacterProp.close();
		ofstream ofstreamDefault(strFileLoc + "Default.RDconf", ios::out | ios::trunc);
		for (auto it = DefaultDice.begin(); it != DefaultDice.end(); ++it)
		{
			ofstreamDefault << it->first << " " << it->second << std::endl;
		}
		ofstreamDefault.close();

		ofstream ofstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf", ios::out | ios::trunc);
		for (auto it = WelcomeMsg.begin(); it != WelcomeMsg.end(); ++it)
		{
			while (it->second.find(' ') != string::npos)
				it->second.replace(it->second.find(' '), 1, "{space}");
			while (it->second.find('\t') != string::npos)
				it->second.replace(it->second.find('\t'), 1, "{tab}");
			while (it->second.find('\n') != string::npos)
				it->second.replace(it->second.find('\n'), 1, "{endl}");
			while (it->second.find('\r') != string::npos)
				it->second.replace(it->second.find('\r'), 1, "{enter}");
			ofstreamWelcomeMsg << it->first << " " << it->second << std::endl;
		}
		ofstreamWelcomeMsg.close();
		DefaultDice.clear();
		DisabledGroup.clear();
		DisabledDiscuss.clear();
		DisabledJRRPGroup.clear();
		DisabledJRRPDiscuss.clear();
		DisabledOBGroup.clear();
		DisabledOBDiscuss.clear();
		ObserveGroup.clear();
		ObserveDiscuss.clear();
		strFileLoc.clear();
		s_Count.clear();
		Duellist.clear();
		Clist.clear();
	}
	void EventHandler::HandleExitEvent()
	{
		if (!Enabled)
			return;
		ilInitList.reset();
		Name.reset();
		ofstream ofstreamDisabledGroup(strFileLoc + "DisabledGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledGroup.begin(); it != DisabledGroup.end(); ++it)
		{
			ofstreamDisabledGroup << *it << std::endl;
		}
		ofstreamDisabledGroup.close();

		ofstream ofstreamDisabledDiscuss(strFileLoc + "DisabledDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledDiscuss.begin(); it != DisabledDiscuss.end(); ++it)
		{
			ofstreamDisabledDiscuss << *it << std::endl;
		}
		ofstreamDisabledDiscuss.close();
		ofstream ofstreamDisabledJRRPGroup(strFileLoc + "DisabledJRRPGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledJRRPGroup.begin(); it != DisabledJRRPGroup.end(); ++it)
		{
			ofstreamDisabledJRRPGroup << *it << std::endl;
		}
		ofstreamDisabledJRRPGroup.close();

		ofstream ofstreamDisabledJRRPDiscuss(strFileLoc + "DisabledJRRPDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledJRRPDiscuss.begin(); it != DisabledJRRPDiscuss.end(); ++it)
		{
			ofstreamDisabledJRRPDiscuss << *it << std::endl;
		}
		ofstreamDisabledJRRPDiscuss.close();

		ofstream ofstreams_Count(strFileLoc + "s_Count.RDconf", ios::out | ios::trunc);
		for (auto it = s_Count.begin(); it != s_Count.end(); ++it)
		{
			ofstreams_Count << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it->second[1] << " "
				<< it->second[2] << " "
				<< it->second[3] << " "
				<< it->second[4] << " "
				<< it->second[5] << " "
				<< it->second[6] << " "
				<< it->second[7] << " "
				<< it->second[8] << std::endl;
		}
		ofstreams_Count.close();

		ofstream ofstreamDuellist(strFileLoc + "Duellist.RDconf", ios::out | ios::trunc);
		for (auto it = Duellist.begin(); it != Duellist.end(); ++it)
		{
			ofstreamDuellist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
				<< it->second << std::endl;
		}
		ofstreamDuellist.close();

		ofstream ofstreamClist(strFileLoc + "Clist.RDconf", ios::out | ios::trunc);
		for (auto it = Clist.begin(); it != Clist.end(); ++it)
		{
			for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
			{
				for (auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); ++it2)
				{
					ofstreamClist << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
						<< it1->first << " " << it2->first << " " << it2->second << std::endl;
				}
			}
		}
		ofstreamClist.close();

		ofstream ofstreamUsingname(strFileLoc + "Usingname.RDconf", ios::out | ios::trunc);
		for (auto it = Usingname.begin(); it != Usingname.end(); ++it)
		{
			if (!it->second.empty())
			ofstreamUsingname << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " " << it->second << std::endl;
		}
		ofstreamUsingname.close();

		ofstream ofstreamDisabledHELPGroup(strFileLoc + "DisabledHELPGroup.RDconf", ios::in | ios::trunc);
		for (auto it = DisabledHELPGroup.begin(); it != DisabledHELPGroup.end(); ++it)
		{
			ofstreamDisabledHELPGroup << *it << std::endl;
		}
		ofstreamDisabledHELPGroup.close();

		ofstream ofstreamDisabledHELPDiscuss(strFileLoc + "DisabledHELPDiscuss.RDconf", ios::in | ios::trunc);
		for (auto it = DisabledHELPDiscuss.begin(); it != DisabledHELPDiscuss.end(); ++it)
		{
			ofstreamDisabledHELPDiscuss << *it << std::endl;
		}
		ofstreamDisabledHELPDiscuss.close();

		ofstream ofstreamDisabledOBGroup(strFileLoc + "DisabledOBGroup.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledOBGroup.begin(); it != DisabledOBGroup.end(); ++it)
		{
			ofstreamDisabledOBGroup << *it << std::endl;
		}
		ofstreamDisabledOBGroup.close();

		ofstream ofstreamDisabledOBDiscuss(strFileLoc + "DisabledOBDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = DisabledOBDiscuss.begin(); it != DisabledOBDiscuss.end(); ++it)
		{
			ofstreamDisabledOBDiscuss << *it << std::endl;
		}
		ofstreamDisabledOBDiscuss.close();

		ofstream ofstreamObserveGroup(strFileLoc + "ObserveGroup.RDconf", ios::out | ios::trunc);
		for (auto it = ObserveGroup.begin(); it != ObserveGroup.end(); ++it)
		{
			ofstreamObserveGroup << it->first << " " << it->second << std::endl;
		}
		ofstreamObserveGroup.close();

		ofstream ofstreamObserveDiscuss(strFileLoc + "ObserveDiscuss.RDconf", ios::out | ios::trunc);
		for (auto it = ObserveDiscuss.begin(); it != ObserveDiscuss.end(); ++it)
		{
			ofstreamObserveDiscuss << it->first << " " << it->second << std::endl;
		}
		ofstreamObserveDiscuss.close();
		ofstream ofstreamCharacterProp(strFileLoc + "CharacterProp.RDconf", ios::out | ios::trunc);
		for (auto it = CharacterProp.begin(); it != CharacterProp.end(); ++it)
		{
			for (auto it1 = it->second.cbegin(); it1 != it->second.cend(); ++it1)
			{
				ofstreamCharacterProp << it->first.QQ << " " << it->first.Type << " " << it->first.GrouporDiscussID << " "
					<< it1->first << " " << it1->second << std::endl;
			}
		}
		ofstreamCharacterProp.close();
		ofstream ofstreamDefault(strFileLoc + "Default.RDconf", ios::out | ios::trunc);
		for (auto it = DefaultDice.begin(); it != DefaultDice.end(); ++it)
		{
			ofstreamDefault << it->first << " " << it->second << std::endl;
		}
		ofstreamDefault.close();

		ofstream ofstreamWelcomeMsg(strFileLoc + "WelcomeMsg.RDconf", ios::out | ios::trunc);
		for (auto it = WelcomeMsg.begin(); it != WelcomeMsg.end(); ++it)
		{
			while (it->second.find(' ') != string::npos)
				it->second.replace(it->second.find(' '), 1, "{space}");
			while (it->second.find('\t') != string::npos)
				it->second.replace(it->second.find('\t'), 1, "{tab}");
			while (it->second.find('\n') != string::npos)
				it->second.replace(it->second.find('\n'), 1, "{endl}");
			while (it->second.find('\r') != string::npos)
				it->second.replace(it->second.find('\r'), 1, "{enter}");
			ofstreamWelcomeMsg << it->first << " " << it->second << std::endl;
		}
		ofstreamWelcomeMsg.close();
	}
	/*void EventHandler::HandleAddGroupEvent()
	{
		AddMsgToQueue(Dice::DiceMsg("[CQ:at,qq=master]�յ�����Ⱥ", 812839433, master, Dice::MsgType::Group));
		return;
	}*/
}