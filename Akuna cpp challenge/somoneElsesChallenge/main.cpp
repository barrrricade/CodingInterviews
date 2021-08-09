#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

#pragma pack(push)
#pragma pack(1)
typedef struct Header {
	char     marker[2];
	uint8_t  msg_type;
	uint64_t sequence_id;
	uint64_t timestamp;
	uint8_t  msg_direction;
	uint16_t msg_len;
} Header;
typedef struct EntryFields {
	uint64_t price;
	uint32_t qty;
	char     instrument[10];
	uint8_t  side;
	uint64_t client_assigned_id;
	uint8_t  time_in_force;
	char     trader_tag[3];
	uint8_t  firm_id;
	char     *firm;
} EntryFields;
typedef struct EntryMsg {
	Header      header;
	EntryFields fields;
	char        termination_string[8];
} EntryMsg;


typedef struct AckFields {
	uint32_t order_id;
	uint64_t client_id;
	uint8_t  order_status;
	uint8_t  reject_code;
} AckFields;

typedef struct AckMsg {
	Header      header;
	AckFields   fields;
	char        termination_string[8];
} AckMsg;

typedef struct PartyGroups {
	uint8_t  firm_id;
	char     trader_tag[3];
	uint32_t qty;
} PartyGroups;

typedef struct FillFields {
	uint32_t order_id;
	uint64_t fill_price;
	uint32_t fill_qty;
	uint8_t  no_of_contras;
	PartyGroups *group;
} FillFields;

typedef struct FillMsg {
	Header      header;
	FillFields  fields;
	char        termination_string[8];
} FillMsg;
#pragma pack(pop)

void showHeader(const Header& header)
{
	cout<<header.marker[0]<<header.marker[1]<<" "<<(int)header.msg_type<<" "<<" "<<header.sequence_id<<" "<<header.timestamp<<" "<<(int)header.msg_direction<<" "<<header.msg_len<<" ";
}
void showEntryFields(const EntryFields& entryFields, const int firmLen)
{
	cout<<entryFields.price<<" "<<entryFields.qty<<" ";
	for(int i = 0; i < 10; i++)
		printf("%c", entryFields.instrument[i]);
	cout<<" "<<(int)entryFields.side;
	cout<<" "<<entryFields.client_assigned_id<<" "<<(int)entryFields.time_in_force<<" ";
	for(int i = 0; i < 3; i++)
		printf("%c", entryFields.trader_tag[i]);
	cout<<" "<<(int)entryFields.firm_id;
	for(int i = 0; i < firmLen; i++)
		printf(" %c", entryFields.firm[i]);
}
void showEntryMsg(const EntryMsg& entryMsg)
{
	showHeader(entryMsg.header);
	showEntryFields(entryMsg.fields, entryMsg.header.msg_len - sizeof(EntryFields) + 4 - 8);
	cout<<" "<<"BDBDBDBD"<<endl;
}
void showAckMsg(const AckMsg& ackMsg)
{
	showHeader(ackMsg.header);
	AckFields fields = ackMsg.fields;
	cout<<fields.order_id<<" "<<fields.client_id<<" "<<(int)fields.order_status<<" "<<(int)fields.reject_code<<" "<<"BDBDBDBD"<<endl;
}
void showFillMsg(const FillMsg& fillMsg)
{
	showHeader(fillMsg.header);
	FillFields fields = fillMsg.fields;
	cout<<fields.order_id<<" "<<fields.fill_price<<" "<<fields.fill_qty<<" "<<(int)fields.no_of_contras<<" ";
	for(int i = 0; i < (int)fields.no_of_contras; i++)
	{
		cout<<(int)(fields.group[i].firm_id)<<" "<<fields.group[i].trader_tag[0]<<fields.group[i].trader_tag[1]<<fields.group[i].trader_tag[2]<<" "<<fields.group[i].qty<<" ";
	}
	cout<<"BDBDBDBD"<<endl;
}

string toString(char arr[], int len)
{
	string str;
	for(int i =0; i < len; i++)
		str.push_back(arr[i]);
	return str;
}
string step4(vector<EntryMsg>& entryMsg)
{
	vector<EntryMsg> gfdMsg;
	for(vector<EntryMsg>::iterator iter = entryMsg.begin(); iter != entryMsg.end(); ++iter)
	{
		if(iter->fields.time_in_force == 2)
			gfdMsg.push_back(*iter);
	}

	map<string, int> result;
	for(vector<EntryMsg>::iterator iter = gfdMsg.begin(); iter != gfdMsg.end(); ++iter)
	{
		string key = toString(iter->fields.trader_tag, 3);
		if(result.find(key) != result.end())
			result[key] += iter->fields.qty;
		else
			result[key] = iter->fields.qty;
	}

	string res;
	int maxValue = 0;
	for(auto it = result.begin(); it != result.end(); ++it)
    {
		if(it->second >= maxValue)
		{
			res = it->first;
			maxValue = it->second;
		}
    }
	return res;
}

string step3(vector<FillMsg>& fillMsg)
{
	map<string, int> result;
	for(vector<FillMsg>::iterator iter = fillMsg.begin(); iter != fillMsg.end(); ++iter)
	{
		for(int i = 0; i < iter->fields.no_of_contras; i++)
		{
			PartyGroups group = iter->fields.group[i];
			string key = toString(group.trader_tag, 3);
			if(result.find(key) == result.end())
			{
				result[key] = group.qty;
			}
			else
			{
				result[key] = result[key] + group.qty;
			}
		}
	}
	string res;
	int maxValue = 0;
	for(auto it = result.begin(); it != result.end(); ++it)
    {
		if(it->second >= maxValue)
		{
			res = it->first;
			maxValue = it->second;
		}
    }
	return res;
}
void step5(vector<EntryMsg>& entryMsg)
{
	sort(entryMsg.begin(), entryMsg.end(), [](const EntryMsg& a, const EntryMsg& b) -> bool{
		char left[11]  = {'\0'};
		char right[11] = {'\0'};
		for(int i = 0; i < 10; i++)
		{
			left[i]  = a.fields.instrument[i];
			right[i] = b.fields.instrument[i];
		}
		return bool(string(left) <= string(right));
	});

	map<string, int> result;
	for(std::vector<int>::size_type i = 0; i != entryMsg.size(); i++)
	{
		string key = toString(entryMsg.at(i).fields.instrument, 10);
		if(result.find(key) != result.end())
			result[key] = entryMsg.at(i).fields.qty + result[key];
		else
			result[key] = entryMsg.at(i).fields.qty;
	}
	string res;
	cout<<result.begin()->first<<" : "<<result.begin()->second/2;
	bool flag = true;
	for(auto it = result.begin(); it != result.end(); ++it)
    {
		if(flag)
			flag = false;
		else
			cout<<", "<<it->first<<" : "<<it->second/2;
    }
}
int main(int argc, char* argv[])
{
	if(argc != 2)
		return -1;

	//string fileName = "example_data_file.bin";
	string fileName = argv[1];
	ifstream input(fileName.c_str(), ios::in | ios::binary);

	if(!input.is_open())
	{
		cout<<"File open failed!"<<endl;
		return -1;
	}

	vector<EntryMsg> entryMsgs;
	vector<AckMsg>   ackMsgs;
	vector<FillMsg>  fillMsgs;

	Header header;
	memset(&header, 0, sizeof(header));
	while(input.read((char*)&header, sizeof(header)))
	{
		uint16_t msgLen = header.msg_len;
		char termination_string[8];
		
		if(1 == header.msg_type)
		{
			EntryFields entryFields;
			memset(&entryFields, 0, sizeof(entryFields));
			input.read((char*)&entryFields.price, sizeof(entryFields.price));
			input.read((char*)&entryFields.qty, sizeof(entryFields.qty));
			input.read((char*)&entryFields.instrument, sizeof(entryFields.instrument));
			input.read((char*)&entryFields.side , sizeof(entryFields.side));
			input.read((char*)&entryFields.client_assigned_id , sizeof(entryFields.client_assigned_id));
			input.read((char*)&entryFields.time_in_force , sizeof(entryFields.time_in_force));
			input.read((char*)&entryFields.trader_tag , sizeof(entryFields.trader_tag));	
			input.read((char*)&entryFields.firm_id , sizeof(entryFields.firm_id));
			
			uint16_t firmLen = msgLen - sizeof(entryFields) + 4 - 8;
			entryFields.firm = new char[firmLen];
			input.read((char*)entryFields.firm, firmLen);

			input.read((char*)termination_string, sizeof(termination_string));

			EntryMsg entryMsg = {header, entryFields, {'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D'}};
			entryMsgs.push_back(entryMsg);
			//showEntryMsg(entryMsg);
		}
		else if (2 == header.msg_type)  //OrderAck 
		{
			AckFields ackFields;
			memset(&ackFields, 0, sizeof(ackFields));
			input.read((char*)&ackFields.order_id, sizeof(ackFields.order_id));
			input.read((char*)&ackFields.client_id, sizeof(ackFields.client_id));
			input.read((char*)&ackFields.order_status, sizeof(ackFields.order_status));
			input.read((char*)&ackFields.reject_code , sizeof(ackFields.reject_code));
			
			input.read((char*)termination_string, sizeof(termination_string));

			AckMsg ackMsg = {header, ackFields, {'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D'}};
			ackMsgs.push_back(ackMsg);
			//showAckMsg(ackMsg);
		}
		else if (3 == header.msg_type)
		{
			FillFields fillFields;
			memset(&fillFields, 0, sizeof(fillFields));
			input.read((char*)&fillFields.order_id, sizeof(fillFields.order_id));
			input.read((char*)&fillFields.fill_price, sizeof(fillFields.fill_price));
			input.read((char*)&fillFields.fill_qty, sizeof(fillFields.fill_qty));
			input.read((char*)&fillFields.no_of_contras, sizeof(fillFields.no_of_contras));

			fillFields.group = (PartyGroups*)malloc(sizeof(PartyGroups) * fillFields.no_of_contras);
			input.read((char*)fillFields.group, sizeof(PartyGroups) * fillFields.no_of_contras);

			input.read((char*)termination_string, sizeof(termination_string));

			FillMsg fillMsg = {header, fillFields, {'B', 'D', 'B', 'D', 'B', 'D', 'B', 'D'}};
			fillMsgs.push_back(fillMsg);
			//showFillMsg(fillMsg);
		}
		else 
		{
		}
		memset(&header, 0, sizeof(header));
	};

	unsigned int total_packets = entryMsgs.size() + ackMsgs.size() + fillMsgs.size();
	unsigned int order_entry_msg_count = entryMsgs.size();
	unsigned int order_ack_msg_count   = ackMsgs.size();
	unsigned int order_fill_msg_count  = fillMsgs.size();
	printf("%u, %u, %u, %u, ", total_packets, order_entry_msg_count, order_ack_msg_count, order_fill_msg_count);
	cout<<step3(fillMsgs)<<", "<<step4(entryMsgs)<<", ";
	step5(entryMsgs);
	cout<<endl;
	return 0;
}
