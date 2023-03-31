#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CVATRegister
{
  private:
    typedef struct TCompany {
      string n;
      string a;
      string name;
      string addr;
      string taxID;
      unsigned int income = 0;
    } TCOM;

    // Function to convert company names to uppercase because it is case-insensitive
    string upperCase(string input) const {
      string result = input;
      for (unsigned int i = 0; i < input.length(); i++) {
        result[i] = toupper(input[i]);
      }

      return result;
    }

    // Comparison function used when searching by tax ID
    static bool cmpID(const TCOM * a, const TCOM * b) {
      if (a->taxID.compare(b->taxID) < 0) {
        return true;
      }
      return false;
    }

    // Comparison function used when searching by name and address
    static bool cmpN(const TCOM * a, const TCOM * b) {
      if (a->n.compare(b->n) < 0) {
        return true;
      }
      else if (a->n.compare(b->n) == 0) {
        if (a->a.compare(b->a) < 0) {
          return true;
        }
      }
      return false;
    }

    TCOM * create(const string & n, const string & a, const string & name, const string & addr, const string & taxID) const {
      TCOM * temp = new TCOM;
      temp->n = n;
      temp->a = a;
      temp->name = name;
      temp->addr = addr;
      temp->taxID = taxID;

      return temp;
    }

  public:
    vector<TCOM *> sortedName;
    vector<TCOM *> sortedID;
    vector<unsigned int> invoices;
    int comCount = 0;
    int invoiceCount = 0;
    bool sorted = false;

    ~CVATRegister() {
      for (int i = 0; i < comCount; i++) {
        delete sortedName[i];
      }
      sortedName.clear();
      sortedID.clear();
    }

    bool newCompany(const string & name, const string & addr, const string & taxID) {

      TCOM * temp = create(upperCase(name), upperCase(addr), name, addr, taxID);

     // Find correct position where to insert company if sorted by name
     auto x = lower_bound(sortedName.begin(), sortedName.end(), temp, cmpN);
      
      // Existing company was already found
      if (x != sortedName.end() && (*x)->n.compare(temp->n) == 0 && (*x)->a.compare(temp->a) == 0) {
        delete temp;
        return false;
      }

      // Find correct position where to insert company by ID
      auto y = lower_bound(sortedID.begin(), sortedID.end(), temp, cmpID);
      if (y != sortedID.end() && (*y)->taxID.compare(temp->taxID) == 0) {
        delete temp;
        return false;
      }

      sortedName.insert(x, temp);
      sortedID.insert(y, temp);
      comCount++;
      
      return true;
    }

    // Delete company by name and address
    bool cancelCompany(const string & name, const string & addr) {

      TCOM * temp = create(upperCase(name), upperCase(addr), name, addr, "");

      auto x = lower_bound(sortedName.begin(), sortedName.end(), temp, cmpN);

      // Try to find company if it exists before deleting it
      if(x == sortedName.end() || (*x)->n.compare(temp->n) != 0 || (*x)->a.compare(temp->a) != 0) {
        delete temp;
        return false;
      }

      temp->taxID = (*x)->taxID;

      auto y = lower_bound(sortedID.begin(), sortedID.end(), temp, cmpID);

      delete (*x);
      
      // Delete company from both vectors
      sortedName.erase(x);
      sortedID.erase(y);
      comCount--;
      
      delete temp;
      return true;
    }

    // Delete company by tax ID
    bool cancelCompany(const string & taxID) {
      TCOM * temp = create("", "", "", "", taxID);

      auto x = lower_bound(sortedID.begin(), sortedID.end(), temp, cmpID);

      if (x == sortedID.end() || (*x)->taxID.compare(taxID) != 0) {
        delete temp;
        return false;
      }

      temp->n = (*x)->n;
      temp->a = (*x)->a;

      auto y = lower_bound(sortedName.begin(), sortedName.end(), temp, cmpN);

      delete (*x);

      sortedID.erase(x);
      sortedName.erase(y);
      comCount--;

      delete temp;
      return true;
    }

    // Add invoice to company by taxID
    bool invoice(const string & taxID, unsigned int amount) {
      TCOM * temp = create("", "", "", "", taxID);

      auto x = lower_bound(sortedID.begin(), sortedID.end(), temp, cmpID);
      if (x == sortedID.end() || (*x)->taxID.compare(taxID) != 0) {
        delete temp;
        return false;
      }

      (*x)->income += amount;
      invoices.push_back(amount);
      invoiceCount++;

      delete temp;
      return true;
    }

    // Add invoice to company by name and address
    bool invoice (const string & name, const string & addr, unsigned int amount) {
      TCOM * temp = create(upperCase(name), upperCase(addr), name, addr, "");

      auto x = lower_bound(sortedName.begin(), sortedName.end(), temp, cmpN);

      if (x == sortedName.end() || (*x)->n.compare(temp->n) != 0 || (*x)->a.compare(temp->a) != 0) {
        delete temp;
        return false;
      }

      (*x)->income += amount;
      invoices.push_back(amount);
      invoiceCount++;

      delete temp;
      return true;
    }

    // Return total income of company by name and address
    bool audit (const string & name, const string & addr, unsigned int & sumIncome ) const {
      TCOM * temp = create(upperCase(name), upperCase(addr), name, addr, "");

      auto x  = lower_bound(sortedName.begin(), sortedName.end(), temp, cmpN);

      if (x == sortedName.end() || (*x)->n.compare(temp->n) != 0 || (*x)->a.compare(temp->a) != 0) {
        delete temp;
        return false;
      }

      sumIncome = (*x)->income;
      delete temp;
      return true;
    }
    
    // Return total income of company by taxID
    bool audit (const string & taxID, unsigned int & sumIncome ) const {
      TCOM * temp = create("", "", "", "", taxID);

      auto x = lower_bound(sortedID.begin(), sortedID.end(), temp, cmpID);

      if (x == sortedID.end() || (*x)->taxID.compare(temp->taxID) != 0) {
        delete temp;
        return false;
      }

      sumIncome = (*x)->income;
      delete temp;
      return true;
    }

    bool firstCompany (string & name, string & addr ) const {
      if (comCount <= 0) {
        return false;
      }
      
      name = sortedName[0]->name;
      addr = sortedName[0]->addr;
      return true;
    }

    // Returns the company next in order after the supplied company
    bool nextCompany(string & name, string & addr ) const {
      TCOM * temp = create(upperCase(name), upperCase(addr), name, addr, "");

      auto x = lower_bound(sortedName.begin(), sortedName.end(), temp, cmpN);

      if (x == sortedName.end() || (x + 1) == sortedName.end() || (*x)->n.compare(temp->n) != 0 || (*x)->a.compare(temp->a) != 0) {
        delete temp;
        return false;
      }

      name = (*(x + 1))->name;
      addr = (*(x + 1))->addr;
      delete temp;
      return true;
    }

    unsigned int  medianInvoice  ( void ) const {
      if (invoiceCount <= 0) {
        return 0;
      }
      else if (invoiceCount == 1) {
        return invoices[0];
      }
      vector<unsigned int> median (invoices);
      nth_element(median.begin(), median.begin() + median.size() / 2, median.end());
      return median[median.size() / 2];
    }
};

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
