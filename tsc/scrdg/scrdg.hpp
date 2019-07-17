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
    std::vector<std::string> call_seqs;
    std::string documentation;
};

class CppParser
{
public:
    CppParser(boost::filesystem::path source_directory);
    void Parse();
    void PrintSummary();

    const inline std::vector<ClassDoc>&  GetClasses() { return m_classes; }
    const inline std::vector<ModuleDoc>& GetModules() { return m_modules; }
    const inline std::vector<MethodDoc>& GetMethods() { return m_methods; }
private:
    void parse_file(const boost::filesystem::path& file);
    void parse_doctext(std::string text);
    void parse_doctype_class(const std::string& classname, const std::string& text);
    void parse_doctype_module(const std::string& modulename, const std::string& text);
    void parse_doctype_method(const std::string& methodname, const std::string& text);

    boost::filesystem::path m_source_dir;
    bool m_docblock_open;
    int m_lino;
    std::string m_doctext;

    std::vector<ClassDoc> m_classes;
    std::vector<ModuleDoc> m_modules;
    std::vector<MethodDoc> m_methods;
};

class Generator
{
public:
    Generator(boost::filesystem::path output_dir,
              boost::filesystem::path template_file,
              const std::string& tsc_version,
              const std::string& tsc_gitrevision,
              const std::vector<ClassDoc>& classes,
              const std::vector<ModuleDoc>& modules,
              const std::vector<MethodDoc>& methods);

    void Generate();
private:
    void generate_classmod(const std::string& type, const std::string& name, const std::string& documentation);
    void generate_class(const ClassDoc& klass);
    void generate_module(const ModuleDoc& mod);
    void generate_indexfile();
    void filter_methods(const std::string& classmodname, std::vector<MethodDoc>& cmethods, std::vector<MethodDoc>& imethods);
    std::string idclean(std::string str);
    std::string tsc_version_str();

    const std::vector<ClassDoc>&  m_classes;
    const std::vector<ModuleDoc>& m_modules;
    const std::vector<MethodDoc>& m_methods;
    boost::filesystem::path m_output_dir;
    const std::string& m_tsc_version;
    const std::string& m_tsc_gitrevision;
    std::string m_template;
};

std::string strip(std::string str);
