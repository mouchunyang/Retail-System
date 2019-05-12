# Amazon Retail System

This is an amazon system that supports features such as LOGIN/LOGOUT, AND/OR searches, ADD to cart, ADD/VIEW reviews, etc.
There is a menu provided with the inputs format given once you run the program. The program prints guidance to the console and updates the database once you quit the program. The sample database is database2.txt.

## Getting Started

Clone the repository to any Linux based laptops. Make sure you don't clone it in existing git repository.

## Running the tests

Run ```make``` and ```./amazon database2.txt```. Perform the tasks you want after LOGIN with the correct password. Instead of storing real passwords, the database only stores the hash code of the passwords.
The passwords of the existing users are given below
```
johnvn	password
aturing	graphics
adal	piazza
mzuckerb abc123	
ttrojan	fighton
billg	csci104
```
You can add more users with different passwords or change the passwords to whatever you want. Just make sure the hash code in the input database matches the output of function convertToHash in hash.cpp.

## Deployment

This program is a real simple version of a Retail System. But it is well-organized and easily improved by adding more features to it such as MySQL and HTTP which will make it an "Online system". 

