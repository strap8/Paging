#include <iostream>
#include <vector>
#include "PageTable.h"
#include <iomanip>
using namespace std;

PageTable::PageTable(int log, int phys){
// constructor
// Utilize a pageMap vector to simulate dynamic memory allocation and access.
// Assumes: log is the number of pages in the page table
//          phys is the number of frames in physical memory
// Results: creates and initializes the entries in the page table
	numStored = 0;
	pageMap.resize(log); // Maximum size of the vector
	for (int i = 0; i < log; i++)
	{
		pageMap.at(i).frameNumber = 0;
		pageMap.at(i).valid = false;
		pageMap.at(i).timeStamp = 0;
	}
	//iter_swap(pageMap.begin(), pageMap.begin() + 1);
	
	for (int i = phys - 1; i >= 0; i--) {
		freeFrames.push_back(i); // Allocate room for the physical memory
	}
}

bool PageTable::isValid(int pageNum){
// Assumes: 0 <= pageNum < page table size
// Returns: true if valid bit is set for that index, else false

	return pageMap[pageNum].valid;
}

void PageTable::accessPage(int pageNum){
// Assumes: 0 <= pageNum < page table size
// Returns: the frame number where pageNum is stored

	cout << pageNum << ": page found at frame " << pageMap.at(pageNum).frameNumber << endl;
}

void PageTable::storePage(int pageNum){
// Assumes: 0 <= pageNum < page table size
// Results: stores pageNum in a free frame, updating the table

	int frameNum;
	if (freeFrames.empty()) {
		int swapPage = selectSwapPage();
		pageMap[swapPage].valid = false;
		frameNum = pageMap[swapPage].frameNumber;
		cout << pageNum << ": PAGE FAULT -- swapping page " << pageNum << " into frame "
			<< frameNum << endl << "   ";
	}
	else {
		frameNum = freeFrames.back(); // frameNum = freeFrames last element
		freeFrames.pop_back(); // Delete last element
		cout << pageNum << ": PAGE FAULT -- inserting page " << pageNum << " into frame "
			<< frameNum << endl << "   ";
	}

	pageMap.at(pageNum).frameNumber = frameNum;
	pageMap[pageNum].timeStamp = numStored; // Increment time stamp
	pageMap[pageNum].valid = true;

	numStored++; // Increment time stamp counter
	printTables();
}

int PageTable::selectSwapPage() {
// Assumes: at least one page is stored in the table
// Returns: index of the oldest page in the table

	int oldPage, oldTime = numStored;
	for (int i = 0; i < pageMap.size(); i++) { // Iterate through the pageMap vector
		if (pageMap[i].valid && pageMap[i].timeStamp < oldTime) { // If the bit is set and time stamp is the smallest
			oldTime = pageMap[i].timeStamp;
			oldPage = i;
		}
	}
	return oldPage;
}


void PageTable::printTables() {
	cout << "Page" << "   Valid" << "   TimeStamp" << "               Free Frames" << endl 
		<< "   |--------------------------------|" << "         |--------------------------------|" << endl;
	for (unsigned int i = 0; i < pageMap.size(); i++)
	{
		cout << left << i << right << setw(6) << pageMap.at(i).frameNumber << setw(6)
			<< pageMap.at(i).valid << setw(6)
			<< pageMap.at(i).timeStamp 
			<< "                 ";
		if (i < freeFrames.size()) {
			cout << right << setw(9) << i << setw(16) << freeFrames.at(i) << endl << "   |--------------------------------|" 
				<< "         |--------------------------------|" << endl;
		}
		else {
			cout << endl << "   |--------------------------------|" << endl;
		}
	}

}
