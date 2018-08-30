#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <list>
#include <cctype>

//
// addKeywordLinks <list_of_files>
//
// This program will read the list of files and for each one found, 
// will find all the keywords and for each one found, will add a reference
// link to allow links to that keyword from other files.
// 
// Keywords are found as follows, in the source files:
//
//      Keyword_name
//      ============
//
// It's the double underline that indicates a keyword. There are some that we 
// might not want, like TODO for example, so that/those will be filtered out.
//

using std::string;
using std::getline;
using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::fill;
using std::list;
using std::ifstream;


// Things I need to ignore if found as headings.
string lookFor[] = {"TODO",
                    "Keywords A", "Keywords B", "Keywords C", "Keywords D", 
                    "Keywords E", "Keywords F", "Keywords G", "Keywords H", 
                    "Keywords I", "Keywords J", "Keywords K", "Keywords L", 
                    "Keywords M", "Keywords N", "Keywords O", "Keywords P", 
                    "Keywords Q", "Keywords R", "Keywords S", "Keywords T", 
                    "Keywords U", "Keywords V", "Keywords W", "Keywords X", 
                    "Keywords Y", "Keywords Z", "Keywords Other" 
};

// Map the above into a string list.
list<string> lookForThese(lookFor, lookFor + sizeof(lookFor) / sizeof(string));

// Globals. Yes, I know...
string this_line;
string last_line;

void doFile(const char *fname);
void doKeywords();


int main (int argc, char *argv[])
{
    for (int x = 1; x < argc; x++) {
        doFile(argv[x]);
    }
    return 0;
}


void doFile(const char *fname)
{
    // Open a file, read it, extract the keywords.

    bool firstLine = true;

    ifstream ifs (fname, std::ifstream::in);
    while (ifs.good()) {
        getline(ifs, this_line);
        if (!firstLine) {
            if (!this_line.empty()) {
                if (this_line.at(0) == '=') {
                    // We have a Keyword in last_line
                    doKeywords();
                }
            }
        } else {
            firstLine = false;
        }
        
        cout << last_line << endl;        
        last_line = this_line;
    }

    cout << this_line;
    
    ifs.close();
}



void doKeywords()
{
    // Blank line?
    if (last_line.empty())
        return;
    
    // Ignore contents of lookForThese list.
    for (list<string>::iterator thisOne = lookForThese.begin(); thisOne != lookForThese.end(); thisOne++)
    {
        string temp = *thisOne;
        if (temp == last_line) {
            // Ignore, it's in our exceptions table.
            return;
        }
    }
    
    // Some keywords begin with an underscore. Leading underscores cause 
    // errors in the link generation. They can be replaced with hyphens 
    // though - that works.
    //
    // In the meantime, output the following:
    //
    // A blank line.
    // A reference link destination (.. _name:) without escaped characters.
    // Also, any '$' will be replaced with 'dlr' and any '%' will get 'pct'.
    // Another blank line.
    // On return, last_line will be output too.
    //
    // No-one said it would be easy. Sigh.
    //
    // END WHEN and END_WHEN create exactly the same link, bugger!
    // I now need to differentiate between spaces and underscores in a keyword.
    
    cout << endl;    
    cout << "..  _";
    
    // Avoid outputting escape character (\).
    // Replace underscores with hyphens.
    for (int x=0; x < last_line.size(); x++) {
        char c = tolower(last_line[x]);
        if (c != '\\') {
            // Get rid of underscores and spaces.
            // '_' becomes '-'
            // ' ' becomes '--'.
            if (c == '_')
                c = '-';
            else
                if (c == ' ') {
                    c = '-';
                    cout << c; // And again below.
                }
            
            // Check for dollar and percent.
            if (c == '$')
                cout << "-dlr";
            else
                if (c == '%')
                    cout << "-pct";
                else
                    cout << c;
        }
    }
    cout << ':' << endl;
    
    cout << endl;
}