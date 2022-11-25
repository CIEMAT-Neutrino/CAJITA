#ifndef JSONFILE_H
#define JSONFILE_H

#include "json.hpp"
#include <iostream>
#include <fstream>


// Para usar el mapa escribir:
// const Json_file map_of_runs=Json_file("mi_mapa.json");
// Y luego loopear en el objeto json_map como un iterable, por ejemplo:
// for (auto i:map_of_runs.json_map) {
//     if (i["threshold"]>50 && i["threshold"]!=""){cout<<i["run"]<<"  "<< i["threshold"]<<endl;}

class Json_file
{
public:
    nlohmann::json json_map;

    Json_file(){};
    Json_file(std::string map_file)
    {
        std::ifstream i(map_file);
        i >> json_map;
    }
};

static bool check_json_file(std::string jsonpath)
{
    //Check file exists:
    std::ifstream f(jsonpath.c_str());
    if (!f.good()) throw std::runtime_error("The Json file provided doesn't exist or can't be accessed");
    return true;
}

static bool check_is_file_type(std::string file_path,std::string file_type=".json")
{
    //Check file is a json:
    if (file_path.find(file_type) != std::string::npos) return true;
    else return false;
}

#endif