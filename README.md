🏦 Bank System

A console-based Bank Management System built with C++ using file handling.

🚀 Latest Updates
🔐 Login System
Added a Login Screen for user authentication.
Users must enter a valid username and password before accessing the system.
Added a permissions system to control what each user can access.
Added Logout functionality.
👥 Manage Users

Added a complete Manage Users Menu with:

List Users – Display all system users.
Add New User – Create a new user and assign permissions.
Delete User – Delete an existing user.
Update User – Update the user's password and permissions.
Find User – Search for a user by username.
🔑 User Permissions

Implemented a bitwise permissions system that allows assigning specific permissions to each user, including:

Show Client List
Add New Client
Delete Client
Update Client
Find Client
Transactions
Manage Users

The permissions are stored as a bit mask using bitwise OR (|) and checked using bitwise AND (&).

🛠️ Technologies Used
C++
File Handling
Structs
Enums
Vectors
Functions
Bitwise Operators
Input/Output Streams
📁 Data Files

The system uses text files to store data:

BankFile.txt – Stores client data.
UsersFile.txt – Stores user accounts and permissions.
📌 Previous Features

The system also supports:

Add New Client
Delete Client
Update Client
Find Client
Show All Clients
Deposit
Withdraw
Total Balances
User Authentication
User Management
Permission-Based Access Control
Logout
