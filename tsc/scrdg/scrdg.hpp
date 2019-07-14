/***************************************************************************
 * scrdg.hpp - Scripting API Documentation Generator (header)
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

#include <string>
#include <boost/filesystem.hpp>

struct ClassDoc
{
    std::string name;
    std::string documentation;
};

struct ModuleDoc
{
    std::string name;
    std::string documentation;
};

struct MethodDoc
{
    std::string name;
    std::string classname;
    bool is_instance_method;
    std::string call_seqs;
    std::string documentation;
};

class Parser
{
public:
    Parser(boost::filesystem::path source_directory);
    void Parse();
    void PrintSummary();

    const inline std::vector<ClassDoc>&  GetClasses() { return m_classes; }
    const inline std::vector<ModuleDoc>& GetModules() { return m_modules; }
    const inline std::vector<MethodDoc>& GetMethods() { return m_methods; }
private:
    void parse_file(const boost::filesystem::path& file);
    void parse_doctext(const std::string& text);
    void parse_doctype_class(const std::string& classname);
    void parse_doctype_module(const std::string& modulename);
    void parse_doctype_method(const std::string& methodname);

    std::vector<ClassDoc> m_classes;
    std::vector<ModuleDoc> m_modules;
    std::vector<MethodDoc> m_methods;
};

class Generator
{
public:
    Generator(boost::filesystem::path output_dir,
              boost::filesystem::path template_file,
              std::vector<ClassDoc> classes,
              std::vector<ModuleDoc> modules,
              std::vector<MethodDoc> methods);

    void Generate();
private:
    void generate_class(const ClassDoc& klass);
    void generate_module(const ModuleDoc& mod);
    void generate_method(const MethodDoc& method);
};
