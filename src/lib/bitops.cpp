
//
//   authors: Steve Tate, Owen Astrachan
//   Bitstream I/O routines.
//   Feb 4, 1993 -- srt
//   modified by ola to ensure bits written in "fifo" order
//   (thus can be read in different chunks than written and
//   get same bit stream
//
//   ported to C++ by Owen Astrachan 3/27/94

#include <iostream>
#include "bitops.h"


const int BITS_PER_WORD = 8;

// bit stuff

// allows a specific bit to be masked from a number
// usage: bmask[k] has the rightmost k bits == 1, other bits 0
//
static long bmask[] = { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff,
		       0x1ff,0x3ff,0x7ff,0xfff,0x1fff,0x3fff,0x7fff,0xffff,
		       0x1ffff,0x3ffff,0x7ffff,0xfffff,0x1fffff,0x3fffff,
		       0x7fffff,0xffffff,0x1ffffff,0x3ffffff,0x7ffffff,
		       0xfffffff,0x1fffffff,0x3fffffff,0x7fffffff,0xffffffff};

static int
pickbit[] = {0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,
             0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000,
	     0x10000,0x20000,0x40000,0x80000,0x1000000,0x2000000};

int kthbit(int x, int k)
// postcondition: returns k-th bit of x
{
    return x & pickbit[k];
}


// output bit stream class

obstream::obstream()
// postcondition: unusable but initialized bit-stream object     
{
    myOutStream = NULL;                       // initialize to no stream
    myOutBuff = 0;                     // nothing in buffer
    myBitsToGo = BITS_PER_WORD;        // bits to go before buffer filled
}

obstream::obstream(const string & name)
// postcondition : bitstream  bound to file specified by name
{
    myOutStream = new ofstream(name.c_str(),ios::binary|ios::out);
    myOutBuff = 0;
    myBitsToGo = BITS_PER_WORD;
}

void obstream::writebits(int numbits, int value)
// postcondition: rightmost numbits bits of value written (buffered)
{

    if (myOutStream == NULL){
		return;         // can't do anything with no stream
    }

    value &= bmask[numbits];    // only right most bits are valid
                                // bug fixed 12/12/96, Scot Ransbottom
    
    while (numbits >= myBitsToGo){
		myOutBuff = (myOutBuff << myBitsToGo) | (value >> (numbits - myBitsToGo));
		myOutStream->put(char(myOutBuff));
		value &= bmask[numbits - myBitsToGo];	
		numbits -= myBitsToGo;
		myBitsToGo = BITS_PER_WORD;
		myOutBuff = 0;
    }
    
    if (numbits > 0){
		myOutBuff = (myOutBuff << numbits) | value;
		myBitsToGo -= numbits;
    }
}

void obstream::flushbits()
// postcondition: unwritten (buffered) bits are flushed     
{

    if (myOutStream == NULL){
	    return;          // can't do anythign with no stream
    }
    
    if (myBitsToGo != BITS_PER_WORD) {
		myOutStream->put( char(myOutBuff << myBitsToGo) );
		myOutBuff = 0;
		myBitsToGo = BITS_PER_WORD;
    }
}

bool obstream::open(const string & name)
// postcondition: output bit stream set to file specified by name
//                any open bit-stream is closed (not flushed)     
{
    if (myOutStream && myOutStream->good()){
		myOutStream->close();
		delete myOutStream;
    }
    myOutStream = new ofstream(name.c_str(),ios::binary|ios::out);
    myOutBuff = 0;
    myBitsToGo = BITS_PER_WORD;    
    return myOutStream && myOutStream->good();
}

obstream::~obstream()
// postcondition: close things up, free memory     
{
    if (myOutStream){
		flushbits();
		myOutStream->close();
		delete myOutStream;
    }
}

void obstream::close()
// postcondition: close the output bit stream          
{
    flushbits();
    if (myOutStream) myOutStream->close();
}

// input bit-stream class

ibstream::ibstream()
// postcondition: unusable but initialized bit-stream object
{
    myInputStream = NULL;
    resetbits();
}

bool ibstream::open(const string & name)
// postcondition: input bit stream set to file specified by name
//                any open bit-stream is closed
{
    if (myInputStream && myInputStream->good()){
		myInputStream->close();
		delete myInputStream;
    }
    myInputStream = new ifstream(name.c_str(),ios::binary|ios::in);
    resetbits();
    return myInputStream && myInputStream->good();
}

ibstream::ibstream(const string & name)
// postcondition: input bit-stream tied to file specified by name     
{
    myInputStream = new ifstream(name.c_str(),ios::binary|ios::in);
    resetbits();
}


ibstream::~ibstream()
// postcondition: close and free up memory     
{
    if (myInputStream){
		myInputStream->close();
		delete myInputStream;
    }
}

void ibstream::close()
// postcondition: close the input bit stream     
{
    if (myInputStream) myInputStream->close();
}


void ibstream::resetbits()
// postcondition: no bits in input buffer, buffer cleared     
{
    myInBuff = myInbbits = 0;
}

bool ibstream::readbits(int numbits,int & retval)
// postcondition: numbits bits read (buffered)
//                and returned as value
//                returns true if read succeeds, otherwise false     
{
    retval = 0;

    if (myInputStream == NULL){
	    return false;          // can't do anything with no stream
    }
    
    while (numbits > myInbbits){
		retval |= myInBuff << (numbits - myInbbits);
		numbits -= myInbbits;
		if ((myInBuff = myInputStream->get()) == EOF){
		    return false;
		}
		myInbbits = BITS_PER_WORD;
    }
    
    if (numbits > 0){
	retval |= myInBuff >> (myInbbits - numbits);
	myInBuff &= bmask[myInbbits - numbits];
	myInbbits -= numbits;
    }
    return true;
}


void ibstream::rewind()
{
    if (myInputStream){
	    myInputStream->clear();     // clear state (e.g., if EOF encounted)
		myInputStream->seekg(0L);  // reset pointer to beginning
		resetbits();
    }

}
