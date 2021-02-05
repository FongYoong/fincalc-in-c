## Financial calculator written in C
👉 [Download (Windows)](https://github.com/FongYoong/fincalc-in-c/releases/download/1.0/fincalc.exe)

***
## Building from source
* Only compatible with **Windows**.
* Prerequisites:
    * [Make](http://gnuwin32.sourceforge.net/packages/make.htm)
    * [gcc](https://gcc.gnu.org/install/binaries.html)
* Clone this repository.
* Open up a terminal in the same folder and run:
    `make`
* [makefile](https://github.com/FongYoong/fincalc-in-c/tree/master/src/makefile) assumes that [gcc](https://gcc.gnu.org/install/binaries.html) is installed and accessible from the command line.
* The resulting program will be located in the build directory.
* Enter `make run` to easily run the program.

***
## Features:
* [FAQs](https://github.com/FongYoong/fincalc-in-c/tree/master/faqs) contain a few PDFs describing the equations used in the financial modelling.
* ✅ User profiles
* ✅ Global interest rates
* ✅ General Loans
    * Interest only
    * Interest & Monthly Payments
    * Car Loan
* ✅ Savings
    * Fixed Deposit
    * Deposit Goals
* ✅ Investments
    * CAGR
    * NPV, IRR
* ✅ Parameter estimation for some of the above categories
* ✅ Procedural graphs as shown below:

![Image](https://i.imgur.com/2QqOli3.png)