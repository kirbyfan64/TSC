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

namespace fs = boost::filesystem;

/**************************************
 * Class implementations
 *************************************/

Parser::Parser(fs::path source_directory)
{
}

void Parser::Parse()
{
}

void Parser::PrintSummary()
{
    std::cout << "=== SUMMARY ===" << std::endl;
    std::cout << "Classes: " << m_classes.size() << std::endl;
    std::cout << "Modules: " << m_modules.size() << std::endl;
    std::cout << "Methods: " << m_methods.size() << std::endl;
}

void Parser::parse_file(const boost::filesystem::path& file)
{
}

void Parser::parse_doctext(const std::string& text)
{
}

void Parser::parse_doctype_class(const std::string& classname)
{
}

void Parser::parse_doctype_module(const std::string& modulename)
{
}

void Parser::parse_doctype_method(const std::string& methodname)
{
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
