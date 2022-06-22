#ifndef JSON_TTT_CPP
#define JSON_TTT_CPP

//Read and save Game states.
#include <iostream>
#include <jsoncpp/json/json.h>
//#include "TTT.cu"


//TTT*TTT_Object
Json::Value* JSON(){
  Json::Value event ;

Json::Value vec(Json::arrayValue);
vec.append(Json::Value(1));
vec.append(Json::Value(2));
vec.append(Json::Value(3));

(event)["competitors"]["home"]["name"] = "Liverpool";
(event)["competitors"]["away"]["code"] = 89223;
(event)["competitors"]["away"]["name"] = "Aston Villa";
(event)["competitors"]["away"]["code"] = vec;

  std::cout << event << std::endl;

    //delete event;
  return NULL;
}

void Save(std::string LogPath){
/*
Json::Value event;
Json::Value vec(Json::arrayValue);
vec.append(Json::Value(1));
vec.append(Json::Value(2));
vec.append(Json::Value(3));

event["competitors"]["home"]["name"] = "Liverpool";
event["competitors"]["away"]["code"] = 89223;
event["competitors"]["away"]["name"] = "Aston Villa";
event["competitors"]["away"]["code"]=vec;

std::cout << event << std::endl;*/
}



/*
TTT* Read_TTT_JSON(std::string LogPath){

  return 0;
}*/


#endif //JSON_TTT_CPP
