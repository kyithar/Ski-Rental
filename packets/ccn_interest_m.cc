//
// Generated file, do not edit! Created by opp_msgc 4.4 from packets/ccn_interest.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ccn_interest_m.h"

USING_NAMESPACE

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




ccn_interest_Base::ccn_interest_Base(const char *name, int kind) : ::cPacket(name,kind)
{
    this->hops_var = 0;
    this->target_var = -1;
    this->rep_target_var = -1;
    this->btw_var = 0;
    this->TTL_var = 10000;
    this->nfound_var = false;
    this->capacity_var = 0;
    this->origin_var = -1;
    this->Delay_var = 0;
    this->serialNumber_var = 0;
    this->aggregate_var = true;
    this->i_type_var = 0;
    this->custo_var = -1;
}

ccn_interest_Base::ccn_interest_Base(const ccn_interest_Base& other) : ::cPacket(other)
{
    copy(other);
}

ccn_interest_Base::~ccn_interest_Base()
{
}

ccn_interest_Base& ccn_interest_Base::operator=(const ccn_interest_Base& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ccn_interest_Base::copy(const ccn_interest_Base& other)
{
    this->chunk_var = other.chunk_var;
    this->hops_var = other.hops_var;
    this->target_var = other.target_var;
    this->rep_target_var = other.rep_target_var;
    this->btw_var = other.btw_var;
    this->TTL_var = other.TTL_var;
    this->nfound_var = other.nfound_var;
    this->capacity_var = other.capacity_var;
    this->origin_var = other.origin_var;
    this->Delay_var = other.Delay_var;
    this->serialNumber_var = other.serialNumber_var;
    this->aggregate_var = other.aggregate_var;
    this->i_type_var = other.i_type_var;
    this->custo_var = other.custo_var;
}

void ccn_interest_Base::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    // field path is abstract -- please do packing in customized class
    doPacking(b,this->chunk_var);
    doPacking(b,this->hops_var);
    doPacking(b,this->target_var);
    doPacking(b,this->rep_target_var);
    doPacking(b,this->btw_var);
    doPacking(b,this->TTL_var);
    doPacking(b,this->nfound_var);
    doPacking(b,this->capacity_var);
    doPacking(b,this->origin_var);
    doPacking(b,this->Delay_var);
    doPacking(b,this->serialNumber_var);
    doPacking(b,this->aggregate_var);
    doPacking(b,this->i_type_var);
    doPacking(b,this->custo_var);
}

void ccn_interest_Base::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    // field path is abstract -- please do unpacking in customized class
    doUnpacking(b,this->chunk_var);
    doUnpacking(b,this->hops_var);
    doUnpacking(b,this->target_var);
    doUnpacking(b,this->rep_target_var);
    doUnpacking(b,this->btw_var);
    doUnpacking(b,this->TTL_var);
    doUnpacking(b,this->nfound_var);
    doUnpacking(b,this->capacity_var);
    doUnpacking(b,this->origin_var);
    doUnpacking(b,this->Delay_var);
    doUnpacking(b,this->serialNumber_var);
    doUnpacking(b,this->aggregate_var);
    doUnpacking(b,this->i_type_var);
    doUnpacking(b,this->custo_var);
}

chunk_t& ccn_interest_Base::getChunk()
{
    return chunk_var;
}

void ccn_interest_Base::setChunk(const chunk_t& chunk)
{
    this->chunk_var = chunk;
}

int ccn_interest_Base::getHops() const
{
    return hops_var;
}

void ccn_interest_Base::setHops(int hops)
{
    this->hops_var = hops;
}

int ccn_interest_Base::getTarget() const
{
    return target_var;
}

void ccn_interest_Base::setTarget(int target)
{
    this->target_var = target;
}

int ccn_interest_Base::getRep_target() const
{
    return rep_target_var;
}

void ccn_interest_Base::setRep_target(int rep_target)
{
    this->rep_target_var = rep_target;
}

double ccn_interest_Base::getBtw() const
{
    return btw_var;
}

void ccn_interest_Base::setBtw(double btw)
{
    this->btw_var = btw;
}

int ccn_interest_Base::getTTL() const
{
    return TTL_var;
}

void ccn_interest_Base::setTTL(int TTL)
{
    this->TTL_var = TTL;
}

bool ccn_interest_Base::getNfound() const
{
    return nfound_var;
}

void ccn_interest_Base::setNfound(bool nfound)
{
    this->nfound_var = nfound;
}

int ccn_interest_Base::getCapacity() const
{
    return capacity_var;
}

void ccn_interest_Base::setCapacity(int capacity)
{
    this->capacity_var = capacity;
}

int ccn_interest_Base::getOrigin() const
{
    return origin_var;
}

void ccn_interest_Base::setOrigin(int origin)
{
    this->origin_var = origin;
}

double ccn_interest_Base::getDelay() const
{
    return Delay_var;
}

void ccn_interest_Base::setDelay(double Delay)
{
    this->Delay_var = Delay;
}

int ccn_interest_Base::getSerialNumber() const
{
    return serialNumber_var;
}

void ccn_interest_Base::setSerialNumber(int serialNumber)
{
    this->serialNumber_var = serialNumber;
}

bool ccn_interest_Base::getAggregate() const
{
    return aggregate_var;
}

void ccn_interest_Base::setAggregate(bool aggregate)
{
    this->aggregate_var = aggregate;
}

int ccn_interest_Base::getI_type() const
{
    return i_type_var;
}

void ccn_interest_Base::setI_type(int i_type)
{
    this->i_type_var = i_type;
}

int ccn_interest_Base::getCusto() const
{
    return custo_var;
}

void ccn_interest_Base::setCusto(int custo)
{
    this->custo_var = custo;
}

class ccn_interestDescriptor : public cClassDescriptor
{
  public:
    ccn_interestDescriptor();
    virtual ~ccn_interestDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(ccn_interestDescriptor);

ccn_interestDescriptor::ccn_interestDescriptor() : cClassDescriptor("ccn_interest", "cPacket")
{
}

ccn_interestDescriptor::~ccn_interestDescriptor()
{
}

bool ccn_interestDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ccn_interest_Base *>(obj)!=NULL;
}

const char *ccn_interestDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ccn_interestDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 15+basedesc->getFieldCount(object) : 15;
}

unsigned int ccn_interestDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<15) ? fieldTypeFlags[field] : 0;
}

const char *ccn_interestDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "path",
        "chunk",
        "hops",
        "target",
        "rep_target",
        "btw",
        "TTL",
        "nfound",
        "capacity",
        "origin",
        "Delay",
        "serialNumber",
        "aggregate",
        "i_type",
        "custo",
    };
    return (field>=0 && field<15) ? fieldNames[field] : NULL;
}

int ccn_interestDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "path")==0) return base+0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chunk")==0) return base+1;
    if (fieldName[0]=='h' && strcmp(fieldName, "hops")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "target")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "rep_target")==0) return base+4;
    if (fieldName[0]=='b' && strcmp(fieldName, "btw")==0) return base+5;
    if (fieldName[0]=='T' && strcmp(fieldName, "TTL")==0) return base+6;
    if (fieldName[0]=='n' && strcmp(fieldName, "nfound")==0) return base+7;
    if (fieldName[0]=='c' && strcmp(fieldName, "capacity")==0) return base+8;
    if (fieldName[0]=='o' && strcmp(fieldName, "origin")==0) return base+9;
    if (fieldName[0]=='D' && strcmp(fieldName, "Delay")==0) return base+10;
    if (fieldName[0]=='s' && strcmp(fieldName, "serialNumber")==0) return base+11;
    if (fieldName[0]=='a' && strcmp(fieldName, "aggregate")==0) return base+12;
    if (fieldName[0]=='i' && strcmp(fieldName, "i_type")==0) return base+13;
    if (fieldName[0]=='c' && strcmp(fieldName, "custo")==0) return base+14;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ccn_interestDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "chunk_t",
        "int",
        "int",
        "int",
        "double",
        "int",
        "bool",
        "int",
        "int",
        "double",
        "int",
        "bool",
        "int",
        "int",
    };
    return (field>=0 && field<15) ? fieldTypeStrings[field] : NULL;
}

const char *ccn_interestDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int ccn_interestDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 0: return pp->getPathArraySize();
        default: return 0;
    }
}

std::string ccn_interestDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getPath(i));
        case 1: {std::stringstream out; out << pp->getChunk(); return out.str();}
        case 2: return long2string(pp->getHops());
        case 3: return long2string(pp->getTarget());
        case 4: return long2string(pp->getRep_target());
        case 5: return double2string(pp->getBtw());
        case 6: return long2string(pp->getTTL());
        case 7: return bool2string(pp->getNfound());
        case 8: return long2string(pp->getCapacity());
        case 9: return long2string(pp->getOrigin());
        case 10: return double2string(pp->getDelay());
        case 11: return long2string(pp->getSerialNumber());
        case 12: return bool2string(pp->getAggregate());
        case 13: return long2string(pp->getI_type());
        case 14: return long2string(pp->getCusto());
        default: return "";
    }
}

bool ccn_interestDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 0: pp->setPath(i,string2long(value)); return true;
        case 2: pp->setHops(string2long(value)); return true;
        case 3: pp->setTarget(string2long(value)); return true;
        case 4: pp->setRep_target(string2long(value)); return true;
        case 5: pp->setBtw(string2double(value)); return true;
        case 6: pp->setTTL(string2long(value)); return true;
        case 7: pp->setNfound(string2bool(value)); return true;
        case 8: pp->setCapacity(string2long(value)); return true;
        case 9: pp->setOrigin(string2long(value)); return true;
        case 10: pp->setDelay(string2double(value)); return true;
        case 11: pp->setSerialNumber(string2long(value)); return true;
        case 12: pp->setAggregate(string2bool(value)); return true;
        case 13: pp->setI_type(string2long(value)); return true;
        case 14: pp->setCusto(string2long(value)); return true;
        default: return false;
    }
}

const char *ccn_interestDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        "chunk_t",
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<15) ? fieldStructNames[field] : NULL;
}

void *ccn_interestDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_interest_Base *pp = (ccn_interest_Base *)object; (void)pp;
    switch (field) {
        case 1: return (void *)(&pp->getChunk()); break;
        default: return NULL;
    }
}


