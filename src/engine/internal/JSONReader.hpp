#ifndef INTERNAL_JSONREADER_HPP
#define INTERNAL_JSONREADER_HPP

#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

/*
 * @file JSONReader.hpp
 * @brief Thin abstraction layer over nlohmann::json
 *
 * @details The class has a json object and access functions which allows users of the class to extract values out of the
 *          open JSON file. The values are provided in a "save" way, meaning that in the case there isn't actually a value 
 *          at the requested field, the program will not crash.
 *
 * @author Felix Hommel
 * @date Nov 23, 2024
*/
class JSONReader
{
	public:
        /** Constrcutors / Destructor */
        JSONReader() : m_json(nullptr) {}
		~JSONReader() = default;

		JSONReader(const JSONReader&) = default;
		JSONReader(JSONReader&&) = delete;
		JSONReader& operator=(const JSONReader&) = default;
		JSONReader& operator=(JSONReader&&) = delete;

        /*
         * @brief Open a json file
         *
         * @param filepath, path to a JSON file
         *
         * @return bool, returns true if file could be read successfully, and false otherwise
         *
         * @details Function to open a JSON file. The function checks for file reading and json parsing errors.
         *          if something goes wrong with reading or parsing, it will return false only if everything succedes
         *          it will return true. The function does not throw itself it only prints a error message on the terminal.
        */
        bool open(const std::string& filepath)
        {
            /** Check if there is already a JSON object loaded, if yes delete it */
            if(!m_json.is_null())
                m_json.clear();

            std::ifstream stream;
            stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
		    {
				stream.open(filepath);
				m_json = nlohmann::json::parse(stream);

                stream.close();
			}
			catch(const std::ios_base::failure& e)
			{
				std::cerr << "ERROR opening file. Error code #" << e.code() << '\n' << e.what() << std::endl;
                
                if(stream.is_open())
                    stream.close();
                else if(!m_json.is_null())
                    m_json.clear();

                return false;
            }
            catch(const nlohmann::json::parse_error& e)
            {
                std::cerr << "ERROR parsing json. Error code #" << e.id << '\n' << e.what() << std::endl;

                if(stream.is_open())
                    stream.close();
                else if(!m_json.is_null())
                    m_json.clear();

                return false;
            }

            if(m_json.is_discarded())
            {
                std::cerr << "ERROR invalid json, object discarded." << std::endl;

                if(!m_json.is_null())
                    m_json.clear();

                return false;
            }

            return true;
        }

        bool readFromString(const std::string& jsonString)
        {
			try
		    {
				m_json = nlohmann::json::parse(jsonString);
			}
            catch(const nlohmann::json::parse_error& e)
            {
                std::cerr << "ERROR parsing json. Error code #" << e.id << '\n' << e.what() << std::endl;

                if(!m_json.is_null())
                    m_json.clear();

                return false;
            }

            if(m_json.is_discarded())
            {
                std::cerr << "ERROR invalid json, object discarded." << std::endl;

                if(!m_json.is_null())
                    m_json.clear();

                return false;
            }

            return true;
        }

        /*
         * @brief Return objects of a variable type that are at the field called fieldName
         *
         * @param fieldName, name of the field of which the value should be retrieved
         *
         * @return std::optional<T>, if the requested field exists return it's value othereise return nothing
         *
         * @details Check if the field that the user of the class wants to retrieve from the JSON object. If the field doest not
         *          exist return a emoty std::optional, otherwise return the value at the specified field.
        */
		template<typename T>
		std::optional<T> get(const std::string& fieldName)
		{
            if(m_json.contains(fieldName))
                return std::make_optional<T>(m_json.at(fieldName));

            return std::nullopt;
		}

	private:
        /** Private member variables */
		nlohmann::json m_json;
};

#endif //!INTERNAL_JSONREADER_HPP
