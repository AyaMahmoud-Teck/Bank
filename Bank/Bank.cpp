#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
const string ClientsFileName = "BankFile.txt";
const string UsersFileName = "UsersFile.txt";
/*
	out : write mode it delete all data and start writing and it creat a file if the file is not exit
	app : append mode add data to the previous data
	in  : read mode Just Read data from the file
*/
struct stClient
{
	string Name;
	string Phone;
	string AccountNumber;
	string PinCode;
	double AccountBalance;
	bool MarkForDeleted = false;
};
struct stUser
{
	string Name = "";
	string Password = "";
	short Permissions = 0;
	bool MarkForDeleted = false;
};
enum enMainMenueOption { eShowClientList = 1, eAddNewClient, eDeleteClient, eUpdateClient, eFindClient, eTransactions, eManageUsers, eLogout };
enum enTransactionMenueOption { eDeposit = 1, eWithdraw, eTotalBalances, eMainMenue };
enum enManageUsersMenueOption { eListUsers = 1, eAddNewUser, eDeleteUser, eUpdateUser, eFindUser, e1MainMenue };
enum enValuesOfPermissions { epShowClientList = 1, epAddNewClient = 2, epDeleteClient = 4, epUpdateClient = 8, epFindClient = 16, epTransactions = 32, epManageUsers = 64 };
stClient ConvertLineToRecord(string sLine, string Delim = "#//#");
void LoginScreen();
//HelpingFunctions
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
stClient ConvertLineToRecord(string sLine, string Delim)
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
string ReadAccountNumber()
{
	string AccountNumber;
	cout << "Please enter Account Number: ";
	cin >> AccountNumber;
	return AccountNumber;
}
//why  when i write const vclients give me an error
bool FindClientByAccountNumber(string AccountNumber, vector<stClient>& vClients, stClient& Client)
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
bool IsClientExistByAccountNumber(string AccountNumber)
{
	fstream Clients;
	string Line;
	stClient Client;

	Clients.open(ClientsFileName, ios::in);
	if (Clients.is_open())
	{
		while (getline(Clients, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				Clients.close();
				return true;
			}
		}
		Clients.close();
	}
	return false;
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
void PrintAllClientsCard()
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
//Add Clients to file
vector<stClient> LoadClientsDataFromFile(string FileName)
{
	vector<stClient>vRecords;

	fstream Clients;
	Clients.open(FileName, ios::in);

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
stClient ReadNewClient()
{
	stClient Client;

	cout << "Please enter AccountNumber:  ";
	getline(cin >> ws, Client.AccountNumber);

	while (IsClientExistByAccountNumber(Client.AccountNumber))
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
void AddNewClient()
{
	stClient Client;

	Client = ReadNewClient();

	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
	////to update the vector with the new client "better"
	//vClients.push_back(Client);
}
void AddNewClients()
{

	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "\n\nAdding new client:\n";

		AddNewClient();
		//It's not the better solution because pushback the client to the vector is better i will put "better" before it 
		//vClients = LoadClientsDataFromFile(ClientsFileName);

		cout << "The Client has been added successfully , do you want to add more clients Y/N: ";
		cin >> AddMore;
	} while (tolower(AddMore) == 'y');

}
//Print list of all clients
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
bool DeleteClientByAccountNumber(vector<stClient>& vClients, string AccountNumber)
{

	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientInfo(Client);

		if (EnsureChoice("Are you sure you want delete this Client Y/N : "))
		{
			MarkForDeleteByAccountNumber(AccountNumber, vClients);

			SaveClientsDataToFile(vClients, ClientsFileName);

			//Refresh Clients
			vClients = LoadClientsDataFromFile(ClientsFileName);

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
bool UpdateClientByAccountNumber(vector<stClient>& vClients, string AccountNumber)
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

			SaveClientsDataToFile(vClients, ClientsFileName);


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
// Main menue options
void ClientsListScreen()
{
	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	ShowListOfAllClients(vClients);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");

}
void AddNewClientsScreen()
{

	cout << "============================================\n";
	cout << right << setw(30) << "Add New Client Screen\n";
	cout << "============================================\n\n";

	AddNewClients();

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}
void DeleteClientScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Delete Client Screen\n";
	cout << "============================================\n\n";

	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	DeleteClientByAccountNumber(vClients, AccountNumber);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");

}
void UpdateClientInfoScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "UpdateClientInfoScreen\n";
	cout << "============================================\n\n";

	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);

	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(vClients, AccountNumber);

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}
void FindClientScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "FindClientScreen\n";
	cout << "============================================\n\n";

	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);
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
void ErrorScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Error\n";
	cout << "============================================\n\n";

	cout << "Please enter valid choice form 1 to 8 \n\n";
	cout << "Press any key to go back to Main Menue...";

	system("pause>0");
}
//Transaction Menue Options
bool DepositBalanceToClientByAccountNumber(vector<stClient>& vClients, string AccountNumber, double AmountOfTransaction)
{
	if (EnsureChoice("Are you sure you want perform this transaction? y/n"))
	{
		for (stClient& Client : vClients)
		{
			if (Client.AccountNumber == AccountNumber)
			{
				Client.AccountBalance += AmountOfTransaction;
				SaveClientsDataToFile(vClients, ClientsFileName);
				cout << "Done Successfully,New balance is  " << Client.AccountBalance << endl << endl;
				return true;
			}
		}
	}
	return false;
}
void DepositScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Deposit Screen\n";
	cout << "============================================\n\n";

	stClient Client;
	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "Client with [ " << AccountNumber << " ] doesn't exist:\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientInfo(Client);

	double DepositAmount = 0;
	cout << "Please enter deposit amount:";
	cin >> DepositAmount;

	DepositBalanceToClientByAccountNumber(vClients, AccountNumber, DepositAmount);

	cout << "Press any key to go back to Transaction Menue...";
	system("pause>0");
}
void WithdrawScreen()

{
	cout << "============================================\n";
	cout << right << setw(30) << "Withdraw Screen\n";
	cout << "============================================\n\n";

	vector<stClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	stClient Client;
	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "Client with [ " << AccountNumber << " ] doesn't exist:\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientInfo(Client);

	double withdrawAmount = 0;
	cout << "Please enter withdraw amount:";
	cin >> withdrawAmount;
	while (withdrawAmount > Client.AccountBalance)
	{
		cout << "Amount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount?";
		cin >> withdrawAmount;
	}

	DepositBalanceToClientByAccountNumber(vClients, AccountNumber, withdrawAmount * -1);

	cout << "Press any key to go back to Transaction Menue...";
	system("pause>0");
}
void BalancesListHeader(short NumberOfClients)
{
	cout << "                                     " << "Client List(" << NumberOfClients << ") Client(s)" << endl;
	cout << "__________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "Account Number ";
	cout << "| " << left << setw(30) << "Client Name ";
	cout << "| " << left << setw(10) << "Balance ";
	cout << endl;
	cout << "__________________________________________________________________________________________\n\n";
}
void PrintClientBalanceRecord(stClient Client)
{
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(30) << Client.Name;
	cout << "| " << left << setw(10) << Client.AccountBalance;
}
void TotalBalancesScreen()
{
	vector<stClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	BalancesListHeader(vClients.size());

	double TotalBalances = 0;
	if (vClients.size() == 0)
	{
		cout << "There is no Clients in the system yet\n";
	}
	else
	{
		for (stClient& Client : vClients)
		{
			TotalBalances += Client.AccountBalance;
			PrintClientBalanceRecord(Client);
			cout << endl;
		}
	}
	cout << "\n\t\t\tTotal Balances = " << TotalBalances << endl << endl;
	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}
// Transaction Menue Screen
enTransactionMenueOption ReadTransactionMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 4]? ";
	cin >> Choice;

	return enTransactionMenueOption(Choice);
}
void ShowTransactionsMenueScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Transaction Menue Screen\n";
	cout << "============================================\n";
	cout << setw(5) << "[1]  Deposit.\n";
	cout << setw(5) << "[2]  Withdraw.\n";
	cout << setw(5) << "[3]  Total Balances.\n";
	cout << setw(5) << "[4]  Main Menue.\n";
	cout << "============================================\n\n";
}
void TransactionMenueScreen()
{
	bool Flag = 1;
	enTransactionMenueOption TransactionMenueOption;

	while (Flag)
	{
		system("cls");
		ShowTransactionsMenueScreenHeader();
		TransactionMenueOption = ReadTransactionMenueOption();

		switch (TransactionMenueOption)
		{
		case enTransactionMenueOption::eDeposit:
		{
			system("cls");
			DepositScreen();
		}
		break;
		case enTransactionMenueOption::eWithdraw:
		{
			system("cls");
			WithdrawScreen();
		}
		break;
		case enTransactionMenueOption::eTotalBalances:
		{
			system("cls");
			TotalBalancesScreen();
		}
		break;
		case enTransactionMenueOption::eMainMenue:
		{
			Flag = 0;
		}
		break;
		}

	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//Helping Function in Manage users menue
string ReadUserName()
{
	string Username;
	cout << "Enter user name:  ";
	cin >> Username;
	return Username;
}
string ReadUserPassword()
{
	string Password = "";
	cout << "Enter user password:  ";
	cin >> Password;
	return Password;
}
stUser ReadUserData()
{
	stUser User;
	User.Name = ReadUserName();
	User.Password = ReadUserPassword();
	return User;
}
string ConvertUserRecordToLine(stUser User, string Delim = "#//#")
{
	string Str = "";

	Str += User.Name + Delim;
	Str += User.Password + Delim;
	Str += to_string(User.Permissions) + Delim;

	return Str;
}
stUser ConvertUserLineToRecord(string sLine, string Delim = "#//#")
{
	vector<string> vUsersData = SplitString(sLine, Delim);
	stUser User;

	User.Name = vUsersData[0];
	User.Password = vUsersData[1];
	User.Permissions = stoi(vUsersData[2]);

	return User;
}
vector<stUser>LoadUsersDataFromFile(string FileName)
{
	vector<stUser>vRecords;
	fstream Users;
	Users.open(FileName, ios::in);
	if (Users.is_open())
	{
		stUser User;
		string Line;
		while (getline(Users, Line))
		{
			if (Line != "")
			{
				User = ConvertUserLineToRecord(Line);
				vRecords.push_back(User);
			}
		}
		Users.close();
	}
	return vRecords;
}
bool IsUsernameAndPasswordCorrect(stUser User)
{
	vector<stUser>Users = LoadUsersDataFromFile(UsersFileName);

	for (stUser& User1 : Users)
	{
		if (User1.Name == User.Name && User1.Password == User.Password)
			return true;
	}
	return false;
}
bool IsUserExistByUsername(string Username)
{
	fstream Users;
	string Line;
	stUser User;

	Users.open(UsersFileName, ios::in);
	if (Users.is_open())
	{
		while (getline(Users, Line))
		{
			User = ConvertUserLineToRecord(Line);
			if (User.Name == Username)
			{
				Users.close();
				return true;
			}
		}
		Users.close();
	}
	return false;
}
bool FindUserByUsername(string UserName, string FileName, stUser& User)
{
	vector<stUser>Users = LoadUsersDataFromFile(FileName);
	for (stUser& User1 : Users)
	{
		if (User1.Name == UserName)
		{
			User = User1;
			return true;
		}
	}
	return false;
}
void PrintUserInfo(stUser User)
{
	cout << "\nThe following is the User data:\n";
	cout << "=============================================================\n";
	cout << "Username       : " << User.Name << endl;
	cout << "Password       : " << User.Password << endl;
	cout << "Permissions    : " << User.Permissions << endl;
	cout << "=============================================================\n\n";
}
short SetPermission(short Permissions, enValuesOfPermissions ValueOfPermission)
{
	return (Permissions | ValueOfPermission);
}
short GetPermissions()
{
	short Permissions = 0;

	if (EnsureChoice("Do you want to give user full access [Y/N]:  "))
	{
		Permissions = -1;
	}
	else
	{
		cout << "Do you want to give access to :\n\n";

		if (EnsureChoice("\nshow client list [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epShowClientList);
		}
		if (EnsureChoice("Add new client [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epAddNewClient);
		}
		if (EnsureChoice(" Delete client [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epDeleteClient);
		}
		if (EnsureChoice("Update client [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epUpdateClient);
		}
		if (EnsureChoice("Find client [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epFindClient);
		}
		if (EnsureChoice("Transactions [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epTransactions);
		}
		if (EnsureChoice("Manage users [Y/N]:  "))
		{
			Permissions = SetPermission(Permissions, enValuesOfPermissions::epManageUsers);
		}
	}
	if (Permissions == 127)
	{
		Permissions = -1;
	}
	return Permissions;
}
bool IsUserHasPermission(enValuesOfPermissions ValueOfPermission, short Permissions)
{
	return((ValueOfPermission & Permissions) == ValueOfPermission);
}
void NoAccessWarning()
{
	cout << "Access Denuied,\n\n";
	cout << " you don't have Permission to do this ,\n";
	cout << " please Contact Your admin\n\n";

	cout << "Press any key to go back to Main Menue...";
	system("pause>0");
}
//Print list of all Users
void PrintUsersListHeader(short NumberOfUsers)
{
	cout << "                                     " << "Users List(" << NumberOfUsers << ") User(s)" << endl;
	cout << "__________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(20) << "Username ";
	cout << "| " << left << setw(10) << "Password ";
	cout << "| " << left << setw(30) << "Permissions ";
	cout << endl;
	cout << "__________________________________________________________________________________________\n\n";
}
void PrintUserRecord(stUser User)
{
	cout << "| " << left << setw(20) << User.Name;
	cout << "| " << left << setw(10) << User.Password;
	cout << "| " << left << setw(30) << User.Permissions;
}
void ShowListOfAllUsers(vector<stUser>vUsersData)
{
	PrintUsersListHeader(vUsersData.size());

	for (stUser& User : vUsersData)
	{
		PrintUserRecord(User);
		cout << endl;
	}

	cout << "\n\n-------------------------------------------------------";
	cout << "------------------------------------\n\n\n";

}
//Add new user
void AddUserDataLineToFile(string FileName, string Line)
{
	fstream Users;
	Users.open(FileName, ios::out | ios::app);

	if (Users.is_open())
	{
		Users << Line << endl;
		Users.close();
	}
}
stUser ReadNewUser()
{
	stUser User;

	cout << "Please enter Username:  ";
	getline(cin >> ws, User.Name);

	while (IsUserExistByUsername(User.Name))
	{
		cout << "User with [ " << User.Name << " ] already exist, Enter another Username? ";
		cin >> User.Name;
	}

	cout << "Please enter Password:  ";
	getline(cin >> ws, User.Password);

	User.Permissions = GetPermissions();

	return User;
}
void AddNewUser()
{
	stUser User;

	User = ReadNewUser();

	AddUserDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}
void AddNewUsers()
{

	char AddMore = 'Y';
	do
	{
		//system("cls");
		cout << "\n\nAdding new Users:\n";

		AddNewUser();
		//It's not the better solution because pushback the client to the vector is better i will put "better" before it 
		//vClients = LoadClientsDataFromFile(ClientsFileName);

		cout << "The User has been added successfully , do you want to add more Users Y/N: ";
		cin >> AddMore;
	} while (tolower(AddMore) == 'y');

}
//Delete User  
bool MarkForDeleteByUsername(string Username, vector<stUser>& vUsers)
{
	for (stUser& User : vUsers)
	{
		if (User.Name == Username)
		{
			User.MarkForDeleted = true;
			return true;
		}
	}
	return false;
}
void SaveUsersDataToFile(vector<stUser>vUsers, string FileName)
{

	fstream Users;

	Users.open(FileName, ios::out);
	string DataLine;

	if (Users.is_open())
	{
		for (stUser& User : vUsers)
		{
			if (User.MarkForDeleted == false)
			{
				DataLine = ConvertUserRecordToLine(User);
				Users << DataLine << endl;
			}
		}
		Users.close();
	}

}
bool DeleteUserByUsername(vector<stUser>& vUsers, string Username)
{

	stUser User;

	if (FindUserByUsername(Username, UsersFileName, User))
	{
		PrintUserInfo(User);

		if (EnsureChoice("Are you sure you want delete this User Y/N : "))
		{
			MarkForDeleteByUsername(Username, vUsers);

			SaveUsersDataToFile(vUsers, UsersFileName);

			//Refresh Users
			vUsers = LoadUsersDataFromFile(UsersFileName);

			cout << "The User with Username ( " << Username << " ) has deleted successfully \n\n";

			return true;
		}
		return false;
	}
	else
	{
		cout << "Client with Username ( " << Username << " ) not found ! :-(\n\n";
		return false;
	}

}
//Update User
stUser ChangeUserRecord(string Username)
{
	stUser User;

	User.Name = Username;

	cout << "Please enter The new password:  ";
	getline(cin >> ws, User.Password);

	User.Permissions = GetPermissions();

	return User;
}
bool UpdateUserByUsername(vector<stUser>& vUsers, string Username)
{
	stUser User;

	if (FindUserByUsername(Username, UsersFileName, User))
	{
		PrintUserInfo(User);

		if (EnsureChoice("Are you sure you want update this User Y/N : "))
		{

			for (stUser& User : vUsers)
			{
				if (User.Name == Username)
				{
					User = ChangeUserRecord(Username);
					break;
				}
			}

			SaveUsersDataToFile(vUsers, UsersFileName);


			cout << "Data updated successfully :-)\n";
			return true;
		}
		return false;
	}
	else
	{
		cout << "Client with Username ( " << Username << " ) not found ! :-(\n\n";
		return false;
	}

}
//Manage users Options
void UsersListScreen()
{
	vector<stUser>vUsers = LoadUsersDataFromFile(UsersFileName);
	ShowListOfAllUsers(vUsers);

	cout << "Press any key to go back to Manage Users Menue...";
	system("pause>0");

}
void AddNewUserScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Add New User Screen\n";
	cout << "============================================\n\n";

	AddNewUsers();

	cout << "Press any key to go back to Manage Users Menue...";
	system("pause>0");
}
void DeleteUserScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Delete User Screen\n";
	cout << "============================================\n\n";

	vector<stUser>vUsers = LoadUsersDataFromFile(UsersFileName);

	string Username = ReadUserName();
	if (Username == "Admin")
	{
		cout << "Sorry no one can delete Admin\n\n";
	}
	else
	{
		DeleteUserByUsername(vUsers, Username);
	}

	cout << "Press any key to go back to Manage Users Menue...";
	system("pause>0");

}
void UpdateUserInfoScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "UpdateUserInfoScreen\n";
	cout << "============================================\n\n";

	vector<stUser>vUsers = LoadUsersDataFromFile(UsersFileName);

	string Username = ReadUserName();
	UpdateUserByUsername(vUsers, Username);

	cout << "Press any key to go back to Manage Users Menue ...";
	system("pause>0");
}
void FindUserScreen()
{
	cout << "============================================\n";
	cout << right << setw(30) << "FindUserScreen\n";
	cout << "============================================\n\n";

	vector<stUser>vUsers = LoadUsersDataFromFile(UsersFileName);
	stUser User;
	string Username = ReadUserName();

	if (FindUserByUsername(Username, UsersFileName, User))
	{
		PrintUserInfo(User);
	}
	else
	{
		cout << "User With Username [ " << Username << " ] is not found!\n\n";
	}

	cout << "Press any key to go back to Manage Users Menue...";
	system("pause>0");
}
//Manage Users Menue
void ShowManageUsersScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(20) << "Manage Users Menue Screen\n";
	cout << "============================================\n";

	cout << setw(5) << "[1]  List Users.\n";
	cout << setw(5) << "[2]  Add New User.\n";
	cout << setw(5) << "[3]  Delete User.\n";
	cout << setw(5) << "[4]  Update User .\n";
	cout << setw(5) << "[5]  Find User.\n";
	cout << setw(5) << "[6]  Main Menue.\n";

	cout << "============================================\n\n";
}
enManageUsersMenueOption ReadManageUsersOption()
{
	short Choice;
	cout << "Choose what do you want to do [1 to 6] ?  ";
	cin >> Choice;
	return enManageUsersMenueOption(Choice);
}
void ManageUsersScreen()
{
	enManageUsersMenueOption ManageUsersOption;
	bool Flag = true;
	while (Flag)
	{
		system("cls");
		ShowManageUsersScreenHeader();
		ManageUsersOption = ReadManageUsersOption();
		switch (ManageUsersOption)
		{
		case(enManageUsersMenueOption::eListUsers):
		{
			system("cls");
			UsersListScreen();
		}
		break;
		case(enManageUsersMenueOption::eAddNewUser):
		{
			system("cls");
			AddNewUserScreen();
		}
		break;
		case(enManageUsersMenueOption::eDeleteUser):
		{
			system("cls");
			DeleteUserScreen();
		}
		break;
		case(enManageUsersMenueOption::eUpdateUser):
		{
			system("cls");
			UpdateUserInfoScreen();
		}
		break;
		case(enManageUsersMenueOption::eFindUser):
		{
			system("cls");
			FindUserScreen();
		}
		break;
		case(enManageUsersMenueOption::e1MainMenue):
		{
			system("cls");
			Flag = false;
		}
		break;
		}
	}

}
//Main Menue
enMainMenueOption ReadMainMenueOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 8]? ";
	cin >> Choice;

	return enMainMenueOption(Choice);
}
void ShowMainMenueScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Manin Menue Screen\n";
	cout << "============================================\n";

	cout << setw(5) << "[1]  Show Client List.\n";
	cout << setw(5) << "[2]  Add New Client.\n";
	cout << setw(5) << "[3]  Delete Client.\n";
	cout << setw(5) << "[4]  Update Client Info.\n";
	cout << setw(5) << "[5]  Find Client.\n";
	cout << setw(5) << "[6]  Transactions.\n";
	cout << setw(5) << "[7]  ManageUsers.\n";
	cout << setw(5) << "[8]  Logout.\n";

	cout << "============================================\n\n";
}
void MainMenueScreen(short Permissions)
{
	bool Flag = true;
	enMainMenueOption MainMenueOption;
	while (Flag)
	{
		system("cls");
		ShowMainMenueScreenHeader();

		MainMenueOption = ReadMainMenueOption();

		switch (MainMenueOption)
		{
		case (enMainMenueOption::eShowClientList):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epShowClientList, Permissions))
				ClientsListScreen();
			else
				NoAccessWarning();
		}
		break;
		case (enMainMenueOption::eAddNewClient):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epAddNewClient, Permissions))
				AddNewClientsScreen();
			else
				NoAccessWarning();
		}
		break;
		case (enMainMenueOption::eDeleteClient):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epDeleteClient, Permissions))
				DeleteClientScreen();
			else
				NoAccessWarning();
		}
		break;
		case (enMainMenueOption::eUpdateClient):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epUpdateClient, Permissions))
				UpdateClientInfoScreen();
			else
				NoAccessWarning();
		}
		break;
		case (enMainMenueOption::eFindClient):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epFindClient, Permissions))
				FindClientScreen();
			else
				NoAccessWarning();
		}
		break;
		case(enMainMenueOption::eTransactions):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epTransactions, Permissions))
				TransactionMenueScreen();
			else
				NoAccessWarning();
		}
		break;
		case(enMainMenueOption::eManageUsers):
		{
			system("cls");
			if (IsUserHasPermission(enValuesOfPermissions::epManageUsers, Permissions))
				ManageUsersScreen();
			else
				NoAccessWarning();
		}
		break;
		case (enMainMenueOption::eLogout):
		{
			system("cls");
			Flag = false;
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
//Login Screen
void PrintLoginScreenHeader()
{
	cout << "============================================\n";
	cout << right << setw(30) << "Login Screen\n";
	cout << "============================================\n";
}
void LoginScreen()
{
	while (true)
	{
		PrintLoginScreenHeader();
		stUser User = ReadUserData();

		while (!IsUsernameAndPasswordCorrect(User))
		{
			system("cls");
			PrintLoginScreenHeader();
			cout << "Invalid Username / Password:\n";
			User = ReadUserData();
		}

		FindUserByUsername(User.Name, UsersFileName, User);
		MainMenueScreen(User.Permissions);
	}
}
int main()
{
	LoginScreen();

	return 0;
}