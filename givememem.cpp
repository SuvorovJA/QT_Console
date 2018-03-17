// описание в "givememem.h"

#include "givememem.h"

#if GIVEMEMEM_DEBUG == 1
#include <iostream>
#endif

GiveMeMem::GiveMeMem()
{
    this->isSuccessful = true;
    this->sizeOfHoldedMem = 0;
    this->memholder = nullptr;
#if GIVEMEMEM_DEBUG == 1
    this->dbgMsg("default create: no allocated mem");
#endif
}

GiveMeMem::GiveMeMem(Operation op, void *reqestedMem, int *size) // ALLOCATE, REALLOCATE
{

}

GiveMeMem::GiveMeMem(Operation op, void *reqestedMem, int *, void *copyedMem) // COPYEXT, COPYMEMBER
{

}

bool GiveMeMem::doit(Operation op, void *reqestedMem, int *size) // ALLOCATE, REALLOCATE
{
    switch (op) {
    case GiveMeMem::Operation::ALLOCATE:
        break;
    case GiveMeMem::Operation::REALLOCATE:
        break;
    default:
        this->isSuccessful = false;
#if GIVEMEMEM_DEBUG == 1
        this->dbgMsg("ALLOCATE|REALLOCATE:unnormal state: operation "
                     + this->toStr(op)
                     + " incorrect");
#endif
    }
}

bool GiveMeMem::doit(Operation op, void *reqestedMem, int *, void *copyedMem) // COPYEXT, COPYMEMBER
{
    switch (op) {
    case GiveMeMem::Operation::COPYEXT:
        break;
    case GiveMeMem::Operation::COPYMEMBER:
        break;
    default:
        this->isSuccessful = false;
#if GIVEMEMEM_DEBUG == 1
        this->dbgMsg("COPYEXT|COPYMEMBER:unnormal state: operation "
                     + this->toStr(op)
                     + " incorrect");
#endif
    }
}

// RELEASE ===============================================================================

bool GiveMeMem::doit(Operation op, void *deletedMem) // RELEASE
{
    this->isSuccessful = false;
    if ((op == Operation::RELEASE) && ((deletedMem == this) || (deletedMem == this->memholder))) {
        this->~GiveMeMem();
        //newer returned ?
    } else {
#if GIVEMEMEM_DEBUG == 1
        this->dbgMsg("RELEASE:unnormal state: "
                     + this->toStr(this->memholder)
                     + ", "
                     + this->toStr(deletedMem)
                     + ", or operation "
                     + this->toStr(op)
                     + " incorrect");
#endif
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
#if GIVEMEMEM_DEBUG == 1
        this->dbgMsg("RELEASE:unnormal state: operation "
                     + this->toStr(op)
                     + " incorrect");
#endif
    }
    return this->isSuccessful; // incorrect operation or incorrect pointer
}

// other staff ===============================================================================

bool GiveMeMem::isOk()
{
    return this->isSuccessful;
}

unsigned int GiveMeMem::size()
{
    return this->sizeOfHoldedMem;
}

// destructor ===============================================================================

GiveMeMem::~GiveMeMem()
{
#if GIVEMEMEM_DEBUG == 1
    this->dbgMsg("memholder:"
                 + this->toStr(this->memholder)
                 + " with size="
                 + this->toStr(this->sizeOfHoldedMem)
                 + " deleted");  // вообще не короче прямого использования cout
#endif
    delete this->memholder;
    this->memholder = nullptr;
}

// debugging staff ===============================================================================

#if GIVEMEMEM_DEBUG == 1
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
#endif
