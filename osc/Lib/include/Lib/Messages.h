#ifndef Lib_Message_h
#define Lib_Message_h

#include <Slash/Core/IMessage.h>
#include <Slash/Core/IAction.h>
#include <Slash/Core/IRelation.h>

#include <Lib/Debug.h>

#include <inlib/cast>

namespace Lib {

class BaseMessage : public virtual Slash::Core::IMessage {
public:
  BaseMessage(){Lib::Debug::increment("Lib::BaseMessage");}
  virtual ~BaseMessage(){Lib::Debug::decrement("Lib::BaseMessage");}
};

class ExitMessage : public BaseMessage {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::ExitMessage)
    else return 0;
  }
  virtual std::string message() const {
    return std::string("Lib::ExitMessage.");
  }
public:
  ExitMessage(int aCode = 0):BaseMessage(),fCode(aCode){
    Lib::Debug::increment("Lib::ExitMessage"); 
  }
  virtual ~ExitMessage(){Lib::Debug::decrement("Lib::ExitMessage");}
  int code() const {return fCode;}
private:
  int fCode;
};

class CreateMessage : public BaseMessage {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::CreateMessage)
    else return 0;
  }
  virtual std::string message() const { 
    return std::string("Lib::CreateMessage : \"") + fWhat + std::string("\".");
  }
public:
  CreateMessage(const std::string& aWhat = ""):BaseMessage(),fWhat(aWhat){
    Lib::Debug::increment("Lib::CreateMessage"); 
  }
  virtual ~CreateMessage(){
    Lib::Debug::decrement("Lib::CreateMessage"); 
  }
  const std::string& what() const { return fWhat;}
private:
  std::string fWhat;
};

class DeleteMessage : public BaseMessage {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::DeleteMessage)
    else return 0;
  }
  virtual std::string message() const { 
    return std::string("Lib::DeleteMessage : \"") + fWhat + std::string("\".");
  }
public:
  DeleteMessage(const std::string& aWhat = ""):BaseMessage(),fWhat(aWhat){
    Lib::Debug::increment("Lib::DeleteMessage"); 
  }
  virtual ~DeleteMessage(){
    Lib::Debug::decrement("Lib::DeleteMessage"); 
  }
  const std::string& what() const { return fWhat;}
private:
  std::string fWhat;
};

class ExecuteMessage : public BaseMessage {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::ExecuteMessage)
    else return 0;
  }
  virtual std::string message() const { 
    return std::string("Lib::ExecuteMessage : \"") 
      + fAction.name() + std::string("\".");
  }
public:
  ExecuteMessage(Slash::Core::IAction& aAction)
  :BaseMessage(),fAction(aAction){
    Lib::Debug::increment("Lib::ExecuteMessage"); 
  }
  virtual ~ExecuteMessage(){
    Lib::Debug::decrement("Lib::ExecuteMessage"); 
  }
  Slash::Core::IAction& action() const { return fAction;}
private:
  Slash::Core::IAction& fAction;
};

class RelationMessage : public BaseMessage {
public: //IMessage:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::RelationMessage)
    else return 0;
  }
  virtual std::string message() const {return "";}
public:
  RelationMessage(Slash::Core::IRelation& aRelation)
  :BaseMessage(),fRelation(aRelation){
    Lib::Debug::increment("Lib::RelationMessage"); 
  }
  virtual ~RelationMessage(){
    Lib::Debug::decrement("Lib::RelationMessage"); 
  }
  Slash::Core::IRelation& relation() const { return fRelation;}
private:
  Slash::Core::IRelation& fRelation;
};

class UpdateMessage : public BaseMessage {
public:
  virtual void* cast(const std::string& a_class) const {
    INLIB_IF_CAST(Lib::UpdateMessage)
    else return 0;
  }
  virtual std::string message() const { 
    return std::string("Lib::UpdateMessage : \"") + fWhat + std::string("\".");
  }
public:
  UpdateMessage(const std::string& aWhat = "",
                Slash::Core::IRelation* aRelation = 0)
  :BaseMessage(),fWhat(aWhat),fRelation(aRelation){
    Lib::Debug::increment("Lib::UpdateMessage"); 
  }
  virtual ~UpdateMessage(){
    Lib::Debug::decrement("Lib::UpdateMessage"); 
  }
  const std::string& what() const { return fWhat;}
  Slash::Core::IRelation* relation() const { return fRelation;}
private:
  std::string fWhat;
  Slash::Core::IRelation* fRelation;
};

}

#endif
