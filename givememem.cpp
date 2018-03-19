// описание в "givememem.h"

#include "givememem.h"
#include <cstring>
#include <iostream>

GiveMeMem::GiveMeMem()
{
    this->isSuccessful = true;
    this->sizeOfHoldedMem = 0;
    this->memholder = nullptr;
    if (GiveMeMem::debugEnabled) this->dbgMsg("default create: no allocated mem");
}

GiveMeMem::GiveMeMem(Operation op, const int &size) // ALLOCATE, REALLOCATE
{
    this->doit(op, size);
}

GiveMeMem::GiveMeMem(Operation op, const int &size, void *copyedMem) // COPYEXT, COPYMEMBER
{
    this->doit(op, size, copyedMem);
}

// *ALLOCATE  ===============================================================================

bool GiveMeMem::doit(Operation op, const int &size) // ALLOCATE, REALLOCATE
{
    switch (op) {
    case GiveMeMem::Operation::ALLOCATE:
        this->isSuccessful = this->getMem(size);
        break;

    case GiveMeMem::Operation::REALLOCATE:
        if (size == 0) {
            if (GiveMeMem::debugEnabled) this->dbgMsg("REALLOCATE: with size=0. use RELEASE instead.");
            this->isSuccessful = false;
            return this->isSuccessful;
        }
        if (this->copyMem(this->memholder, size)) {
            this->isSuccessful = true;
        } else {
            this->isSuccessful = false;
        }
        break;

    default:
        this->isSuccessful = false;
        if (GiveMeMem::debugEnabled) this->dbgMsg("ALLOCATE|REALLOCATE:unnormal state: operation "
                    + this->toStr(op)
                    + " incorrect");
    }
    return this->isSuccessful;
}

// COPY* ===============================================================================

bool GiveMeMem::doit(Operation op, const int &size, void *copyedMem) // COPYEXT, COPYMEMBER
{
    switch (op) {
    case GiveMeMem::Operation::COPYMEMBER:
    case GiveMeMem::Operation::COPYEXT:
        if (this->copyMem(copyedMem, size)) {
            this->isSuccessful = true;
        } else {
            this->isSuccessful = false;
        }
        break;
    default:
        this->isSuccessful = false;
        if (GiveMeMem::debugEnabled) this->dbgMsg("COPYEXT|COPYMEMBER:unnormal state: operation "
                    + this->toStr(op)
                    + " incorrect");
    }
    return  this->isSuccessful;
}

// RELEASE ===============================================================================

bool GiveMeMem::doit(Operation op, void *deletedMem) // RELEASE
{
    this->isSuccessful = false;
    if ((op == Operation::RELEASE) && ((deletedMem == this) || (deletedMem == this->memholder))) {
        this->~GiveMeMem();
        //newer returned ?
    } else {
        if (GiveMeMem::debugEnabled) this->dbgMsg("RELEASE:unnormal state: "
                    + this->toStr(this->memholder)
                    + ", "
                    + this->toStr(deletedMem)
                    + ", or operation "
                    + this->toStr(op)
                    + " incorrect");
    }
    return this->isSuccessful; // incorrect operation or incorrect pointer
}

bool GiveMeMem::doit(Operation op) // RELEASE
{
    this->isSuccessful = false;
    if (op == Operation::RELEASE) {
        this->~GiveMeMem();
        //newer returned?
    } else {
        if (GiveMeMem::debugEnabled) this->dbgMsg("RELEASE:unnormal state: operation "
                    + this->toStr(op)
                    + " incorrect");
    }
    return this->isSuccessful; // incorrect operation or incorrect pointer
}

// other staff ===============================================================================

bool GiveMeMem::isOk()
{
    return this->isSuccessful;
}

unsigned int GiveMeMem::getSize()
{
    return this->sizeOfHoldedMem;
}

void *GiveMeMem::access()
{
    return this->memholder;
}


bool GiveMeMem::getMem(const int &size) // allocate mem to memholder
{
    if (size == 0 ) {
        if (GiveMeMem::debugEnabled) this->dbgMsg("getMem():unnormal state: attempt allocate mem with size=0");
        return false;
    } else {
        this->memholder = malloc(size);
        if (memholder == nullptr) {
            if (GiveMeMem::debugEnabled) this->dbgMsg("getMem():unnormal state: fail allocate mem with size=" + this->toStr(size));
            this->sizeOfHoldedMem = 0;
            return false;
        } else {
            if (GiveMeMem::debugEnabled) this->dbgMsg("getMem():succesfull allocate mem for "
                        + this->toStr(this->memholder)
                        + " with size=" + this->toStr(size));
            this->sizeOfHoldedMem = size;
        }
        return this->cleanMem();
    }
}


bool GiveMeMem::cleanMem()  //clean memholder mandatory
{
    bool result;
    if (this->memholder == nullptr) {
        if (GiveMeMem::debugEnabled) this->dbgMsg("cleanMem():unnormal state: attempt clean mem with nullptr");
        result = false;
    } else if (this->sizeOfHoldedMem == 0) {
        if (GiveMeMem::debugEnabled) this->dbgMsg("cleanMem():unnormal state: attempt clean mem with size=0");
        result = false;
    } else {
        this->memholder = memset(this->memholder, 0, this->sizeOfHoldedMem);
        if (memholder == nullptr) {
            if (GiveMeMem::debugEnabled) this->dbgMsg("cleanMem():unnormal state: fail clean mem for "
                        + this->toStr(this->memholder)
                        + " with size=" + this->toStr(sizeOfHoldedMem));
            result = false;
        } else {
            if (GiveMeMem::debugEnabled) this->dbgMsg("cleanMem():succesfull clean mem for "
                        + this->toStr(this->memholder)
                        + " with size=" + this->toStr(sizeOfHoldedMem));
            result = true;
        }
    }
    return result;
}


bool GiveMeMem::copyMem(const void *from, const unsigned int &size)  // copy &size mem from *void to *memholder
{
    void *localBackup = nullptr;
    unsigned int localBackupSize = this->sizeOfHoldedMem;
    bool isSelfcopy = false;
    if (from == nullptr) {
        if (GiveMeMem::debugEnabled) this->dbgMsg("copyMem():unnormal state: attempt nullptr copy with size= "
                    + this->toStr(size));
        return false;
    }
    localBackup = this->memholder;
    if (this->memholder == from) {
        isSelfcopy = true;
    }
    if (this->getMem(size)) { // new zeroed mem in memholder
        if (isSelfcopy && (localBackupSize > size)) { // shrink of memholder allocation
            this->memholder = memcpy(this->memholder, from, size);
        } else if (isSelfcopy && (localBackupSize < size)) { // grow of memholder allocation
            this->memholder = memcpy(this->memholder, from, localBackupSize);
        } else { // other mem
            this->memholder = memcpy(this->memholder, from, size);
        }
        if (this->memholder == nullptr) { //fail
            if (GiveMeMem::debugEnabled) this->dbgMsg("copyMem():unnormal state: failed copy "
                        + this->toStr(from)
                        + " with size=" + this->toStr(size));
            free(this->memholder); //revert
            this->memholder = localBackup; //revert
            return false;
        } else { // success
            if (GiveMeMem::debugEnabled) this->dbgMsg("copyMem():succesfull copy "
                        + this->toStr(from)
                        + " to "
                        + this->toStr(this->memholder)
                        + " with size=" + this->toStr(size));
            this->sizeOfHoldedMem = size;
            return true;
        }
    } else {
        return false;
    }
}



// destructor ===============================================================================

GiveMeMem::~GiveMeMem()
{
    if (GiveMeMem::debugEnabled) this->dbgMsg("memholder:"
                + this->toStr(this->memholder)
                + " with size="
                + this->toStr(this->sizeOfHoldedMem)
                + " deleted");  // вообще не короче прямого использования cout
//  delete this->memholder;  // to free or to delete ?
    free(this->memholder);
    this->memholder = nullptr;
    this->sizeOfHoldedMem = 0; // на всякий случай
    this->isSuccessful = true; // на всякий случай
}

// debugging staff ===============================================================================

/**
  q: undefined reference to static member
  a: You need to define _frequency in the .cpp file.
  */
//static
bool GiveMeMem::debugEnabled;

/**
The static modifier goes in .h, but not in .cpp.

That's because it means two different things in the two contexts.

When used inside a class declaration, static indicates that the method it refers to is a
class method (that you should use without a object reference), rather than a instance
method (that you need a object reference to invoke).

When used inside the implementation file (i.e. outside of a class declaration), it
indicates that the method it refers to should have static linkage, which means it should not
be made visible from outside the object file that contains it. From an object oriented point
of view, these functions are private to the file that contains them. It's pretty obvious that
this cannot work if other files (which are using your class) should access the method.
*/
//static
void GiveMeMem::setDebug(bool state)
{
    GiveMeMem::debugEnabled = state;
}

void GiveMeMem::dbgMsg(const std::string &s)
{
    std::cout << "(GiveMeMem:" << s << ")" << std::endl;
}
std::string GiveMeMem::toStr(void *vp)
{
    std::stringstream tmps;
    tmps << vp;
    std::string result;
    tmps >> result;
    return result;
}
std::string GiveMeMem::toStr(const void *&vp)
{
    std::stringstream tmps;
    tmps << vp;
    std::string result;
    tmps >> result;
    return result;
}
std::string GiveMeMem::toStr(int i)
{
    std::stringstream tmps;
    tmps << i;
    std::string result;
    tmps >> result;
    return result;
}
std::string GiveMeMem::toStr(GiveMeMem::Operation op)
{
    switch (op) {
    case GiveMeMem::Operation::ALLOCATE:
        return "ALLOCATE";
    case GiveMeMem::Operation::COPYEXT:
        return "COPYEXT";
    case GiveMeMem::Operation::COPYMEMBER:
        return "COPYMEMBER";
    case GiveMeMem::Operation::REALLOCATE:
        return "REALLOCATE";
    case GiveMeMem::Operation::RELEASE:
        return "RELEASE";
    default:
        return "";
    }
}

