//#23,24
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
/*
	out : write mode it delete all data and start writing and it creat a file if the file is not exit
	app : append mode add data to the previous data
	in  : read mode Just Read data from the file
*/
const string ClientsFileName = "BankFile.txt";
struct stClient
{
	string Name;
	string Phone;
	string AccountNumber;
	string PinCode;
	double AccountBalance;
	bool MarkForDeleted = false;
};
vector<string> SplitString(string Str, string Delim)//this function devide the string into words and add all words in a vector
{
	vector <string> vString;

	string sWord;
	short Pos = 0;

	while ((Pos = Str.find(Delim)) != std::string::npos)
	{
		sWord = Str.substr(0, Pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		Str.erase(0, Pos + Delim.length());
	}
	if (Str != "")
	{
		vString.push_back(Str);
	}
	return vString;
}
enum enMainMenueChoice { eShowClientList = 1, eAddNewClient, eDeleteClient, eUpdateClient, eFindClient, eExit };

bool IsClientExistByAccountNumber(string AccountNumber, vector<stClient>vClients)
{

	for (stClient& vClient : vClients)
	{
		if (vClient.AccountNumber == AccountNumber)
		{
			return true;
		}
	}
	return false;
}
stClient ReadNewClint(vector<stClient>& vClients)
{
	stClient Client;

	cout << "Please enter AccountNumber:  ";
	getline(cin >> ws, Client.AccountNumber);

	while (IsClientExistByAccountNumber(Client.AccountNumber, vClients))
	{
		cout << "Client with [ " << Client.AccountNumber << " ] already exist, Enter another Account Number? ";
		cin >> Client.AccountNumber;
	}

	cout << "Please enter Pin code:  ";
	getline(cin >> ws, Client.PinCode);

	cout << "Please enter Client Name:  ";
	getline(cin, Client.Name);

	cout << "Please enter Client Phone:  ";
	getline(cin, Client.Phone);


	cout << "Please enter AccountBalance:  ";
	cin >> Client.AccountBalance;

	return Client;
}
string ConvertRecordToLine(stClient Client, string Delim = "#//#")
{
	string Str = "";

	Str += Client.AccountNumber + Delim;
	Str += Client.PinCode + Delim;
	Str += Client.Name + Delim;
	Str += Client.Phone + Delim;
	Str += to_string(Client.AccountBalance);

	return Str;
}
stClient ConvertLineToRecord(string sLine, string Delim = "#//#")
{
	vector<string> vClientData = SplitString(sLine, Delim);
	stClient Client;

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;
}
void PrintClientInfo(stClient Client)
{
	cout << "\nThe following is the client data:\n";
	cout << "=============================================================\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin code       : " << Client.PinCode << endl;
	cout << "Client name    : " << Client.Name << endl;
	cout << "Client phone   : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance << endl;
	cout << "=============================================================\n\n";
}
//Add Clients to file
vector<stClient> LoadClientsDataFromFile(string FileName)
{
	vector<stClient>vRecords;

	fstream Clients;
	Clients.open(ClientsFileName, ios::in);

	if (Clients.is_open())
	{
		stClient Client;
		string Line;

		while (getline(Clients, Line))
		{
			if (Line != "")
			{
				Client = ConvertLineToRecord(Line);
				vRecords.push_back(Client);
			}
		}
		Clients.close();
	}
	return vRecords;
}
void AddDataLineToFile(string FileName, string Line)
{
	fstream Clients;
	Clients.open(FileName, ios::out | ios::app);

	if (Clients.is_open())
	{
		Clients << Line << endl;
		Clients.close();
	}
}
void AddNewClient(vector<stClient>& vClients)
{
	stClient Client;

	Client = ReadNewClint(vClients);

	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
	//to update the vector with the new client "better"
	vClients.push_back(Client);
}
void AddClients(vector<stClient>& vClients)
{

	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "Add new client:\n";

		AddNewClient(vClients);
		//It's not the better solution because pushback the client to the vector is better i will put "better" before it 
		//vClients = LoadClientsDataFromFile(ClientsFileName);

		cout << "The Client has been added successfully , do you want to add more clients Y/N: ";
		cin >> AddMore;
	} while (tolower(AddMore) == 'y');

}
//Read Clients Data from file and print them
void ReadClientsFile()
{
	fstream Clients;
	Clients.open(ClientsFileName, ios::in);

	if (Clients.is_open())
	{
		string Line;

		while (getline(Clients, Line))
		{
			PrintClientInfo(ConvertLineToRecord(Line));
		}
		Clients.close();
	}
}
//Read Clients Data from file and print them
void PrintHeader(short NumberOfClients)
{
	cout << "                                     " << "Client List(" << NumberOfClients << ") Client(s)" << endl;
	cout << "__________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "Account Number ";
	cout << "| " << left << setw(10) << "Pin Code ";
	cout << "| " << left << setw(30) << "Client Name ";
	cout << "| " << left << setw(15) << "Phone ";
	cout << "| " << left << setw(10) << "Balance ";
	cout << endl;
	cout << "__________________________________________________________________________________________\n\n";
}
void PrintClientRecord(stClient Client)
{
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(10) << Client.AccountBalance;
}
//Load Clients Data From File
void ShowListOfAllClients(vector<stClient>vClientsData)
{
	PrintHeader(vClientsData.size());

	for (stClient& Client : vClientsData)
	{
		PrintClientRecord(Client);
		cout << endl;
	}

	cout << "\n\n-------------------------------------------------------";
	cout << "------------------------------------\n\n\n";

}
//
string ReadAccountNumber()
{
	string AccountNumber;
	cout << "Please enter Account Number: ";
	cin >> AccountNumber;
	return AccountNumber;
}
bool FindClientByAccountNumber(string AccountNumber, vector<stClient>vClients, stClient& Client)
{

	for (stClient& vClient : vClients)
	{
		if (vClient.AccountNumber == AccountNumber)
		{
			Client = vClient;
			return true;
		}
	}
	return false;
}
//Delete client  
bool EnsureChoice(string Message)
{
	char Choice = 'n';
	cout << Message << endl;
	cin >> Choice;

	if (tolower(Choice) == 'y')
		return true;

	return false;
}
bool MarkForDeleteByAccountNumber(string AccountNumber, vector<stClient>& vClients)
{
	for (stClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			Client.MarkForDeleted = true;
			return true;
		}
	}
	return false;
}
void SaveClientsDataToFile(vector<stClient>vClients, string FileName)
{

	fstream Clients;

	Clients.open(FileName, ios::out);
	string DataLine;

	if (Clients.is_open())
	{
		for (stClient& Client : vClients)
		{
			if (Client.MarkForDeleted == false)
			{
				DataLine = ConvertRecordToLine(Client);
				Clients << DataLine << endl;
			}
		}
		Clients.close();
	}

}
bool DeleteClientByAccountNumber(vector<stClient>& vClients, string FileName, string AccountNumber)
{

	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientInfo(Client);

		if (EnsureChoice("Are you sure you want delete this Client Y/N : "))
		{
			MarkForDeleteByAccountNumber(AccountNumber, vClients);

			SaveClientsDataToFile(vClients, FileName);

			//Refresh Clients
			vClients = LoadClientsDataFromFile(FileName);

			cout << "The Client with Account number ( " << AccountNumber << " ) has deleted successfully \n\n";

			return true;
		}
		return false;
	}
	else
	{
		cout << "Client with account number ( " << AccountNumber << " ) not found ! :-(\n\n";
		return false;
	}

}
// Update Client
stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Please enter Pin code:  ";
	getline(cin >> ws, Client.PinCode);

	cout << "Please enter Client Name:  ";
	getline(cin, Client.Name);

	cout << "Please enter Client Phone:  ";
	getline(cin, Client.Phone);


	cout << "Please enter AccountBalance:  ";
	cin >> Client.AccountBalance;

	return Client;
}
bool UpdateClientByAccountNumber(vector<stClient>& vClients, string FileName, string AccountNumber)
{
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientInfo(Client);

		if (EnsureChoice("Are you sure you want update this Client Y/N : "))
		{

			for (stClient& Client : vClients)
			{
				if (Client.AccountNumber == AccountNumber)
				{
					Client = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(vClients, FileName);


			cout << "Data updated successfully :-)\n";
			return true;
		}
		return false;
	}
	else
	{
		cout << "Client with account number ( " << AccountNumber << " ) not found ! :-(\n\n";
		return false;
	}

}
// Bank

void ClientsListScreen(vector<stClient>vClients)
{
	ShowListOfAllClients(vClients);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");

}

void AddNewClientScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Add New Client Screen\n";
	cout << "============================================\n\n";
}
void AddNewClientScreen(vector<stClient>& vClients)
{

	AddNewClientScreenHeader();

	AddClients(vClients);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}

void DeleteClientScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Delete Client Screen\n";
	cout << "============================================\n\n";
}
void DeleteClientScreen(vector<stClient>& vClients, string FileName)
{
	DeleteClientScreenHeader();

	string AccountNumber = ReadAccountNumber();

	DeleteClientByAccountNumber(vClients, FileName, AccountNumber);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");

}

void UpdateClientInfoScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "UpdateClientInfoScreen\n";
	cout << "============================================\n\n";
}
void UpdateClientInfoScreen(vector<stClient>& vClients, string FileName)
{
	UpdateClientInfoScreenHeader();

	string AccountNumber = ReadAccountNumber();

	UpdateClientByAccountNumber(vClients, FileName, AccountNumber);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}

void FindClientScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "FindClientScreen\n";
	cout << "============================================\n\n";
}
void FindClientScreen(vector<stClient> vClients, string FileName)
{
	FindClientScreenHeader();

	stClient Client;
	string AccountNumber = ReadAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientInfo(Client);
	}
	else
	{
		cout << "Client With Account Number [ " << AccountNumber << " ] is not found!\n\n";
	}

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}

void ExitScreen()
{
	cout << "-----------------------\n";
	cout << right << setw(18) << "Program ends\n";
	cout << "-----------------------\n";
}

void ErrorScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Error\n";
	cout << "============================================\n\n";
}
void ErrorScreen()
{
	ErrorScreenHeader();

	cout << "Please enter valid choice form 1 to 6 \n\n";
	cout << "Press any key to go back to Main Menue...";

	system("pause>0");
}

enMainMenueChoice ReadChoice()
{
	int Choice;
	cout << "Choose what do you want to do? [1 to 6]? ";
	cin >> Choice;

	return enMainMenueChoice(Choice);
}
void MainMenueScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Manin Menue Screen\n";
	cout << "============================================\n";

	cout << setw(5) << "[1]  Show Client List.\n";
	cout << setw(5) << "[2]  Add New Client.\n";
	cout << setw(5) << "[3]  Delete Client.\n";
	cout << setw(5) << "[4]  Update Client Info.\n";
	cout << setw(5) << "[5]  Find Client.\n";
	cout << setw(5) << "[6]  Exit.\n";

	cout << "============================================\n\n";
}
void MainMenueScreen()
{
	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);

	enMainMenueChoice Choice;
	bool Flag = true;

	while (Flag)
	{
		system("cls");
		MainMenueScreenHeader();
		
		enMainMenueChoice Choice = ReadChoice();		

		switch (Choice)
		{
		case (enMainMenueChoice::eShowClientList) :
		{
			system("cls");
			ClientsListScreen(vClients);
		}
		break;
		case (enMainMenueChoice::eAddNewClient):
		{
			system("cls");
			AddNewClientScreen(vClients);
		}
		break;
		case (enMainMenueChoice::eDeleteClient):
		{
			system("cls");
			DeleteClientScreen(vClients, ClientsFileName);
		}
		break;
		case (enMainMenueChoice::eUpdateClient):
		{
			system("cls");
			UpdateClientInfoScreen(vClients, ClientsFileName);
		}
		break;
		case (enMainMenueChoice::eFindClient):
		{
			system("cls");
			FindClientScreen(vClients, ClientsFileName);
		}
		break;
		case (enMainMenueChoice::eExit):
		{
			Flag = false;
			system("cls");
			ExitScreen();
		}
		break;
		default:
		{
			system("cls");
			ErrorScreen();
		}
		}
	}

}
int main()
{

	MainMenueScreen();

	return 0;
}