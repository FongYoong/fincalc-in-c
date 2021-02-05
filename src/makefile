CC = gcc
CFLAGS  = -Wall 
FILE_NAME  = fincalc.exe

default: fincalc

fincalc:
	$(CC) $(CFLAGS) -o $(FILE_NAME) main.c CustomPrintf.c DataManager.c FileManager.c InputManager.c InterestRates.c Invest.c Loan.c Savings.c cJSON.c tinyfiledialogs.c -LC:/mingw/lib -lcomdlg32 -lole32

