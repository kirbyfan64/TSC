/***************************************************************************
 * scrdg.cpp - Scripting API Documentation Generator
 *
 * Copyright © 2005 - 2011 Florian Richter
 * Copyright © 2012-2019 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scrdg.hpp"
#include <iostream>
#include <sstream>
#include <regex>

namespace fs = boost::filesystem;

/**************************************
 * Class implementations
 *************************************/

Parser::Parser(fs::path source_directory)
    : m_source_dir(source_directory),
      m_docblock_open(false),
      m_lino(0)
{
}

void Parser::Parse()
{
    for(auto iter = fs::recursive_directory_iterator(m_source_dir); iter != fs::recursive_directory_iterator(); iter++) {
        if (iter->path().extension() == fs::path(".cpp") ||
            iter->path().extension() == fs::path(".hpp")) {
            parse_file(*iter);
        }
    }

    struct {
        bool operator() (const ModuleDoc& a, const ModuleDoc& b) { return a.name < b.name; }
        bool operator() (const ClassDoc& a, const ClassDoc& b) { return a.name < b.name; }
        bool operator() (const MethodDoc& a, const MethodDoc& b) { return a.name < b.name; }
    } namesorter;

    std::sort(m_classes.begin(), m_classes.end(), namesorter);
    std::sort(m_modules.begin(), m_modules.end(), namesorter);
    std::sort(m_methods.begin(), m_methods.end(), namesorter);
}

void Parser::PrintSummary()
{
    std::cout << "=== SUMMARY ===" << std::endl;
    std::cout << "Classes: " << m_classes.size() << std::endl;
    std::cout << "Modules: " << m_modules.size() << std::endl;
    std::cout << "Methods: " << m_methods.size() << std::endl;
}

void Parser::parse_file(const boost::filesystem::path& file_path)
{
    std::cout << "Examining " << file_path << std::endl;
    m_lino = 0;

    std::ifstream file(file_path.native());
    for (std::string line; std::getline(file, line); ) {
        m_lino += 1;

        if (m_docblock_open) { // We are in a /** block here
            std::regex r("^\\s*\\*/$");
            if (std::regex_search(line, r)) { // /** block closed by */
                m_docblock_open = false;
                parse_doctext(m_doctext);
                m_doctext.clear();
            }
            else { // Inside open /** block
                std::string text = strip(line);
                // Remove leading "*" or "* ", if any
                if (text[0] == '*' && text[1] == ' ') {
                    text = text.substr(2);
                } else if (text[0] == '*') {
                    text = text.substr(1);
                }

                m_doctext += text + "\n";
            }
        }
        else { // We are not in a /** block here
            std::regex r("^/\\*\\*$"); // Opening block must be in line of its own
            if (std::regex_search(line, r)) { // /** block opened
                m_docblock_open = true;
            } // else ignore the line
        }
    }
}

void Parser::parse_doctext(std::string text)
{
    std::string firstline = text.substr(0, text.find("\n"));

    size_t pos = std::string::npos;
    if ((pos = firstline.find(":")) != std::string::npos) {
        // Remove control line from doc text
        text = text.substr(firstline.length());

        std::string type = firstline.substr(0, pos);
        std::string name = firstline.substr(pos+2, firstline.find("\n")-1);

        if (type == "Method")
            parse_doctype_method(name, text);
        else if (type == "Class")
            parse_doctype_class(name, text);
        else if (type == "Module")
            parse_doctype_module(name, text);
        else
            std::cerr << "Warning: Skipping invalid documentation type '" << type << "' on line " << m_lino << std::endl;
    }
    else {
        std::cerr << "Warning: Skipping invalid documentation comment block on line " << m_lino << std::endl;
    }
}

void Parser::parse_doctype_class(const std::string& classname, const std::string& text)
{
    m_classes.push_back(ClassDoc{name: classname, documentation: text});
}

void Parser::parse_doctype_module(const std::string& modulename, const std::string& text)
{
    m_modules.push_back(ModuleDoc{name: modulename, documentation: text});
}

void Parser::parse_doctype_method(const std::string& methodstr, const std::string& text)
{
    bool is_imethod = false;
    std::string classname;
    std::string methodname;

    // Check whether class or instance method (or invalid)
    size_t pos = std::string::npos;
    if ((pos = methodstr.find("#")) != std::string::npos) {
        is_imethod = true;
        classname  = methodstr.substr(0, pos);
        methodname = methodstr.substr(pos+1);
    }
    else if ((pos = methodstr.find("::")) != std::string::npos) {
        is_imethod = false;
        pos        = methodstr.rfind("::");
        methodname = methodstr.substr(pos+2);
        classname  = methodstr.substr(0, pos);
    }
    else {
        std::cerr << "Warning: Invalid method spec '" << methodstr << "' on line " << m_lino << ". Ignoring." << std::endl;
    }

    // Divide the rest of the documentation block into the call sequences
    // and the real documentation.
    std::stringstream stream(text);
    std::vector<std::string> calls;
    std::string doctext;
    for (std::string line; std::getline(stream, line); ) {
        // Ignore newlines surrounding the call sequences block
        if (line.empty())
            continue;
        if (line[0] == ' ') {
            calls.push_back(strip(line));
            continue;
        }
        // End of call sequences reached
        doctext = line + "\n";
        break;
    }

    // "How to read a stream into a string in C++ as a oneliner?" - "Not possible
    // before C++11, and afterwards you need unreadable angle code".
    // <https://stackoverflow.com/questions/3203452>.
    doctext += std::string(std::istreambuf_iterator<char>(stream), {});

    m_methods.push_back(MethodDoc{name: methodname, classname: classname, is_instance_method: is_imethod, call_seqs: calls, documentation: doctext});
}

Generator::Generator(fs::path output_dir,
                     fs::path template_file,
                     std::vector<ClassDoc> classes,
                     std::vector<ModuleDoc> modules,
                     std::vector<MethodDoc> methods)
{
}

void Generator::Generate()
{
}

void Generator::generate_class(const ClassDoc& klass)
{
}

void Generator::generate_module(const ModuleDoc& mod)
{
}

void Generator::generate_method(const MethodDoc& method)
{
}

/**************************************
 * Global functions
 *************************************/

// Remove leading and trailing spaces from str, returning the changed string.
std::string strip(std::string str)
{
    while (str[0] == ' ')
        str = str.substr(1);

    while (str[str.length()-1] == ' ')
        str = str.substr(0, str.length() - 1);

    return str;
}

void copy_static_contents(const fs::path& tsc_source_dir, const fs::path& target_dir)
{
    std::cout << "Copying static files... ";
    fs::create_directories(target_dir / "graphics");

    fs::copy(tsc_source_dir / "data" / "pixmaps" / "game" / "items" / "cookie.png", target_dir / "graphics" / "cookie.png");
    fs::copy(tsc_source_dir / "data" / "pixmaps" / "enemy" / "furball" / "brown" / "turn.png", target_dir / "graphics" / "furball.png");
    fs::copy(tsc_source_dir / "data" / "pixmaps" / "enemy" / "eato" / "brown" / "3.png", target_dir / "graphics" / "eato.png");
    fs::copy(tsc_source_dir / "data" / "pixmaps" / "enemy" / "flyon" / "orange" / "open_1.png", target_dir / "graphics" / "flyon.png");
    fs::copy(tsc_source_dir / "data" / "pixmaps" / "enemy" / "gee" / "electro" / "5.png", target_dir / "graphics" / "gee.png");
    fs::copy(tsc_source_dir / "docs" / "scripting" / "style.css", target_dir / "style.css");

    std::cout << "Done." << std::endl;
}

void process_core_files(const fs::path& source_dir, const fs::path& target_dir)
{
    std::cout << "Generating scripting core API documentation." << std::endl;

    Parser parser(source_dir  / "src" / "scripting");
    parser.Parse();
    parser.PrintSummary();

    Generator gen(target_dir,
                  source_dir / "docs" / "scripting" / "template.html.part",
                  parser.GetClasses(),
                  parser.GetModules(),
                  parser.GetMethods());
    gen.Generate();
}

void process_ssl_files(const fs::path& source_dir, const fs::path& target_dir)
{
    std::cout << "Doing nothing with the SSL for now" << std::endl;
}

int main(int argc, char* argv[])
{
    std::cout << "TSC scripting documentation generator starting." << std::endl;

    if (argc != 3) {
        std::cerr << "Usage: scrdg OUTPUT_DIR TSC_SOURCE_DIR" << std::endl;
        std::cerr << "(where TSC_SOURCE_DIR refers to the tsc/ directory)" << std::endl;
        return 1;
    }

    fs::path output_dir     = fs::path(argv[1]);
    fs::path tsc_source_dir = fs::path(argv[2]);

    // Prepare output directory
    if (fs::exists(output_dir))
        fs::remove_all(output_dir);
    fs::create_directories(output_dir);

    copy_static_contents(tsc_source_dir, output_dir);
    process_core_files(tsc_source_dir, output_dir);
    //process_ssl_files(tsc_source_dir / "data" / "scripting", output_dir);

    std::cout << "TSC scripting documentation generator finished." << std::endl;
    return 0;
}
