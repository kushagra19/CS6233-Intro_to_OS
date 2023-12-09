#include <iostream>
#include <cstdlib>
#include <ctime>
#include<fstream>

using namespace std;

//node for a page
struct Page 
{   
    /*Stores page number and reference bit for a particular page*/

    int number;
    bool referenceBit;
};

void secondChancePageReplacement(int pageTrace[], int n, int frames, ofstream& outputFile) 
{   
    /*This function implements the second chance page replacement algorithm
      Input : pageTrace, length of page trace, number of memory frames and outputFile to write data points.
      Output : Prints the number of page faults for the given page trace and frame size.
    */

    Page frameTable[frames];
    int front = 0;  // Front of the circular queue
    int tail = 0;   // Rear of the circular queue

    int pageFaults = 0;

    for (int i = 0; i < n; ++i) 
    {   
        //mark page found as false initially
        bool pageFound = false;

        //check for the page in frames table
        for (int j = 0; j < frames; ++j) 
        {   
            //if we find the page in frame table (no page fault)
            if (frameTable[j].number == pageTrace[i]) 
            {
                frameTable[j].referenceBit = true;
                pageFound = true;
                break;
            }
        }
        //page not in our frames table
        if (!pageFound) 
        {   
            //find the victim frame (oldest one for my approach)
            while (frameTable[tail].referenceBit) 
            {
                // Give a second chance to pages with reference bit set
                frameTable[tail].referenceBit = false;
                tail = (tail + 1) % frames;
            }

            // Replace the page at the rear of the circular queue
            frameTable[tail].number = pageTrace[i];
            frameTable[tail].referenceBit = true;
            tail = (tail + 1) % frames;

            //Increment Page Fault
            ++pageFaults;
        }
    }

    cout << "Number of page faults with " << frames << " frames: " << pageFaults << std::endl; 
    outputFile << frames << " " << pageFaults << endl; 
}

int main(int argc, char* argv[]) 
{   
    //error checking for program arguements
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <p>" << std::endl;
        return 1;
    }

    int n = std::atoi(argv[1]);
    int p = std::atoi(argv[2]);

    if (n < 16 || p < 8) 
    {
        cerr << "Invalid parameters. n must be >= 16, and p must be >= 8." << std::endl;
        return 1;
    }

    //declaring array for page trace
    int pageTrace[n];
    srand(time(0)); // Seed for random number generation

    // Generate random page trace
    for (int i = 0; i < n; ++i) 
    {
        pageTrace[i] = rand() % p;
    }

    ofstream outputFile("page_faults_data.dat");

    // Apply second chance page replacement algorithm for different frame sizes
    for (int frames = 4; frames <= p; ++frames) 
    {
        secondChancePageReplacement(pageTrace, n, frames, outputFile);
    }

    outputFile.close();

    // Generate Gnuplot script
    ofstream scriptFile("plot_script.gp");
    scriptFile << "set term qt persist font 'Arial,12'" << endl;  // Open plot in a window
    scriptFile << "set title 'Page Faults vs Number of Frames'" << endl;
    scriptFile << "set xlabel 'Number of Frames'" << endl;
    scriptFile << "set ylabel 'Number of Page Faults'" << endl;
    scriptFile << "plot for [i=0:" << p-4<< "] 'page_faults_data.dat' index i using 1:2 with linespoints pointtype 7 title '(Page Fault, Frames)'" << endl;
    scriptFile.close();

    // Call Gnuplot to execute the script
    system("gnuplot plot_script.gp");

    return 0;
}
