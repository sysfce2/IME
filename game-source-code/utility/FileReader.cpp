#include "utility/FileReader.h"
#include "exceptions/Exceptions.h"

void Utility::FileReader::readFileInto(std::stringstream& buffer, const std::string& filename){
    inFile_.open(filename);
	if(!inFile_.good())
		throw FileNotFound("Cannot find file, " + filename);
	buffer << inFile_.rdbuf(); //Read file content
	inFile_.close();
}