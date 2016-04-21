//
// Generated file, do not edit! Created by opp_msgc 4.4 from packets/ccn_data.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ccn_data_m.h"

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




ccn_data_Base::ccn_data_Base(const char *name, int kind) : ::cPacket(name,kind)
{
    this->price_var = 0;
    this->target_var = -1;
    this->origin_var = -1;
    this->hops_var = 0;
    this->TSB_var = 0;
    this->TSI_var = 0;
    this->capacity_var = 0;
    this->btw_var = 0;
    this->found_var = false;
    this->c_decision_var = -1;
    this->custo_check_var = -1;
    this->pre_reqcost_var = -1;
    this->tresh_var = -1;
}

ccn_data_Base::ccn_data_Base(const ccn_data_Base& other) : ::cPacket(other)
{
    copy(other);
}

ccn_data_Base::~ccn_data_Base()
{
}

ccn_data_Base& ccn_data_Base::operator=(const ccn_data_Base& other)
{
    if (this==&other) return *this;
    ::cPacket::operator=(other);
    copy(other);
    return *this;
}

void ccn_data_Base::copy(const ccn_data_Base& other)
{
    this->chunk_var = other.chunk_var;
    this->price_var = other.price_var;
    this->target_var = other.target_var;
    this->origin_var = other.origin_var;
    this->hops_var = other.hops_var;
    this->TSB_var = other.TSB_var;
    this->TSI_var = other.TSI_var;
    this->capacity_var = other.capacity_var;
    this->btw_var = other.btw_var;
    this->found_var = other.found_var;
    this->c_decision_var = other.c_decision_var;
    this->custo_check_var = other.custo_check_var;
    this->pre_reqcost_var = other.pre_reqcost_var;
    this->tresh_var = other.tresh_var;
}

void ccn_data_Base::parsimPack(cCommBuffer *b)
{
    ::cPacket::parsimPack(b);
    doPacking(b,this->chunk_var);
    doPacking(b,this->price_var);
    doPacking(b,this->target_var);
    doPacking(b,this->origin_var);
    doPacking(b,this->hops_var);
    doPacking(b,this->TSB_var);
    doPacking(b,this->TSI_var);
    doPacking(b,this->capacity_var);
    doPacking(b,this->btw_var);
    doPacking(b,this->found_var);
    doPacking(b,this->c_decision_var);
    doPacking(b,this->custo_check_var);
    doPacking(b,this->pre_reqcost_var);
    doPacking(b,this->tresh_var);
}

void ccn_data_Base::parsimUnpack(cCommBuffer *b)
{
    ::cPacket::parsimUnpack(b);
    doUnpacking(b,this->chunk_var);
    doUnpacking(b,this->price_var);
    doUnpacking(b,this->target_var);
    doUnpacking(b,this->origin_var);
    doUnpacking(b,this->hops_var);
    doUnpacking(b,this->TSB_var);
    doUnpacking(b,this->TSI_var);
    doUnpacking(b,this->capacity_var);
    doUnpacking(b,this->btw_var);
    doUnpacking(b,this->found_var);
    doUnpacking(b,this->c_decision_var);
    doUnpacking(b,this->custo_check_var);
    doUnpacking(b,this->pre_reqcost_var);
    doUnpacking(b,this->tresh_var);
}

chunk_t& ccn_data_Base::getChunk()
{
    return chunk_var;
}

void ccn_data_Base::setChunk(const chunk_t& chunk)
{
    this->chunk_var = chunk;
}

double ccn_data_Base::getPrice() const
{
    return price_var;
}

void ccn_data_Base::setPrice(double price)
{
    this->price_var = price;
}

int ccn_data_Base::getTarget() const
{
    return target_var;
}

void ccn_data_Base::setTarget(int target)
{
    this->target_var = target;
}

int ccn_data_Base::getOrigin() const
{
    return origin_var;
}

void ccn_data_Base::setOrigin(int origin)
{
    this->origin_var = origin;
}

int ccn_data_Base::getHops() const
{
    return hops_var;
}

void ccn_data_Base::setHops(int hops)
{
    this->hops_var = hops;
}

int ccn_data_Base::getTSB() const
{
    return TSB_var;
}

void ccn_data_Base::setTSB(int TSB)
{
    this->TSB_var = TSB;
}

int ccn_data_Base::getTSI() const
{
    return TSI_var;
}

void ccn_data_Base::setTSI(int TSI)
{
    this->TSI_var = TSI;
}

double ccn_data_Base::getCapacity() const
{
    return capacity_var;
}

void ccn_data_Base::setCapacity(double capacity)
{
    this->capacity_var = capacity;
}

double ccn_data_Base::getBtw() const
{
    return btw_var;
}

void ccn_data_Base::setBtw(double btw)
{
    this->btw_var = btw;
}

bool ccn_data_Base::getFound() const
{
    return found_var;
}

void ccn_data_Base::setFound(bool found)
{
    this->found_var = found;
}

int ccn_data_Base::getC_decision() const
{
    return c_decision_var;
}

void ccn_data_Base::setC_decision(int c_decision)
{
    this->c_decision_var = c_decision;
}

int ccn_data_Base::getCusto_check() const
{
    return custo_check_var;
}

void ccn_data_Base::setCusto_check(int custo_check)
{
    this->custo_check_var = custo_check;
}

double ccn_data_Base::getPre_reqcost() const
{
    return pre_reqcost_var;
}

void ccn_data_Base::setPre_reqcost(double pre_reqcost)
{
    this->pre_reqcost_var = pre_reqcost;
}

double ccn_data_Base::getTresh() const
{
    return tresh_var;
}

void ccn_data_Base::setTresh(double tresh)
{
    this->tresh_var = tresh;
}

class ccn_dataDescriptor : public cClassDescriptor
{
  public:
    ccn_dataDescriptor();
    virtual ~ccn_dataDescriptor();

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

Register_ClassDescriptor(ccn_dataDescriptor);

ccn_dataDescriptor::ccn_dataDescriptor() : cClassDescriptor("ccn_data", "cPacket")
{
}

ccn_dataDescriptor::~ccn_dataDescriptor()
{
}

bool ccn_dataDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ccn_data_Base *>(obj)!=NULL;
}

const char *ccn_dataDescriptor::getProperty(const char *propertyname) const
{
    if (!strcmp(propertyname,"customize")) return "true";
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ccn_dataDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 14+basedesc->getFieldCount(object) : 14;
}

unsigned int ccn_dataDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
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
    return (field>=0 && field<14) ? fieldTypeFlags[field] : 0;
}

const char *ccn_dataDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "chunk",
        "price",
        "target",
        "origin",
        "hops",
        "TSB",
        "TSI",
        "capacity",
        "btw",
        "found",
        "c_decision",
        "custo_check",
        "pre_reqcost",
        "tresh",
    };
    return (field>=0 && field<14) ? fieldNames[field] : NULL;
}

int ccn_dataDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chunk")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "price")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "target")==0) return base+2;
    if (fieldName[0]=='o' && strcmp(fieldName, "origin")==0) return base+3;
    if (fieldName[0]=='h' && strcmp(fieldName, "hops")==0) return base+4;
    if (fieldName[0]=='T' && strcmp(fieldName, "TSB")==0) return base+5;
    if (fieldName[0]=='T' && strcmp(fieldName, "TSI")==0) return base+6;
    if (fieldName[0]=='c' && strcmp(fieldName, "capacity")==0) return base+7;
    if (fieldName[0]=='b' && strcmp(fieldName, "btw")==0) return base+8;
    if (fieldName[0]=='f' && strcmp(fieldName, "found")==0) return base+9;
    if (fieldName[0]=='c' && strcmp(fieldName, "c_decision")==0) return base+10;
    if (fieldName[0]=='c' && strcmp(fieldName, "custo_check")==0) return base+11;
    if (fieldName[0]=='p' && strcmp(fieldName, "pre_reqcost")==0) return base+12;
    if (fieldName[0]=='t' && strcmp(fieldName, "tresh")==0) return base+13;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ccn_dataDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "chunk_t",
        "double",
        "int",
        "int",
        "int",
        "int",
        "int",
        "double",
        "double",
        "bool",
        "int",
        "int",
        "double",
        "double",
    };
    return (field>=0 && field<14) ? fieldTypeStrings[field] : NULL;
}

const char *ccn_dataDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ccn_dataDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data_Base *pp = (ccn_data_Base *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string ccn_dataDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data_Base *pp = (ccn_data_Base *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getChunk(); return out.str();}
        case 1: return double2string(pp->getPrice());
        case 2: return long2string(pp->getTarget());
        case 3: return long2string(pp->getOrigin());
        case 4: return long2string(pp->getHops());
        case 5: return long2string(pp->getTSB());
        case 6: return long2string(pp->getTSI());
        case 7: return double2string(pp->getCapacity());
        case 8: return double2string(pp->getBtw());
        case 9: return bool2string(pp->getFound());
        case 10: return long2string(pp->getC_decision());
        case 11: return long2string(pp->getCusto_check());
        case 12: return double2string(pp->getPre_reqcost());
        case 13: return double2string(pp->getTresh());
        default: return "";
    }
}

bool ccn_dataDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data_Base *pp = (ccn_data_Base *)object; (void)pp;
    switch (field) {
        case 1: pp->setPrice(string2double(value)); return true;
        case 2: pp->setTarget(string2long(value)); return true;
        case 3: pp->setOrigin(string2long(value)); return true;
        case 4: pp->setHops(string2long(value)); return true;
        case 5: pp->setTSB(string2long(value)); return true;
        case 6: pp->setTSI(string2long(value)); return true;
        case 7: pp->setCapacity(string2double(value)); return true;
        case 8: pp->setBtw(string2double(value)); return true;
        case 9: pp->setFound(string2bool(value)); return true;
        case 10: pp->setC_decision(string2long(value)); return true;
        case 11: pp->setCusto_check(string2long(value)); return true;
        case 12: pp->setPre_reqcost(string2double(value)); return true;
        case 13: pp->setTresh(string2double(value)); return true;
        default: return false;
    }
}

const char *ccn_dataDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
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
    return (field>=0 && field<14) ? fieldStructNames[field] : NULL;
}

void *ccn_dataDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ccn_data_Base *pp = (ccn_data_Base *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getChunk()); break;
        default: return NULL;
    }
}


