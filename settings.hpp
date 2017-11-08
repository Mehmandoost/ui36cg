#ifndef SETTINGS_H
#define SETTINGS_H
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <stdlib.h>
#include <iostream>
class Settings {
public:
	Settings(std::string file_location) {
		try {
		boost::property_tree::ini_parser::read_ini(file_location, pt);
		} catch (...) {
				std::cout << "could not open config file : " << file_location << "." << std::endl;
			exit(EXIT_FAILURE);
		}
	};
	std::string get_Value(std::string section, std::string var){
		try {
		return this->pt.get<std::string>(section+"."+var);
		} catch(...) {
			std::cout << "could not get: [" << section << "::" << var << "] from config file." << std::endl;
			exit(EXIT_FAILURE);
		}
	};
private:
	boost::property_tree::ptree pt;
};

#endif
