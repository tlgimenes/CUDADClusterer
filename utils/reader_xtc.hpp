/*============================================================================*/
/*! \file reader_xtc.hpp 
 *  \author Tiago LOBATO GIMENES            (tlgimenes@gmail.com)
 *  \date 2015-03-30 18:49 
 *
 *  \brief .xtc file reader 
 *
 *  This file contains the implementation of a class for reading .xtc files 
 *  with a trajectory list
 * */
/*============================================================================*/

///////////////////////////////////////////////////////////////////////////////

#ifndef READER_XTC_HPP
#define READER_XTC_HPP

///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>

#include "error.hpp"

#include "xdrfile/xdrfile.h"
#include "xdrfile/xdrfile_xtc.h"

///////////////////////////////////////////////////////////////////////////////

/*! \brief Maximun file name lenght */
#define MAX_FILE_NAME_LENGTH 128

///////////////////////////////////////////////////////////////////////////////

namespace bfs = boost::filesystem;

///////////////////////////////////////////////////////////////////////////////

/*! \brief Class for reading trajlist and .xtc files */
class reader_xtc
{
    public:
        /*! \brief Reads all trajectories specified in the trajlist file. 
         *
         * A trajlist file can contains the relative paths to the .xtc files or name of
         * files that contains relative paths to the .xtc files. The absolute
         * path is always done in the following way : path = home/trajlist
         * */
        static inline void read_list(const std::string& home, const std::string& trajlist,
                std::vector<float>& data, int& n_atoms);

    protected:
        /*! \brief Reads a trajectory file. 
         *
         * It can be *.xtc or any other file defined in _supported_ext vector 
         * and appends the new trajectories in the data vector
         * */
        static inline void read_trajfile(const std::string& trajfile, std::vector<float>&
                data, int& n_atoms, int& n_samples);

        /*! \brief finds *.xtc files from trajlist
         *
         * Given a trajlist path, this function will recursively try to find
         * the (*.xtc) files and insert it on the framefile_list. The complete
         * path for the file must be given by home/trajlist 
         * */
        static inline void get_framefile_list(std::vector<std::string>& framefile_list,
                const std::string& home, const std::string& trajlist);

        /*! \brief Checks if the extension of file "file_name" is supported or not
         * by this class
         *
         * \return true if file extension is supported, false otherwise
         * */
        static inline bool is_ext_supported(const std::string& file_name);

    private:
        static std::vector<std::string> _supported_ext; /*! list of supported extensions */
};

///////////////////////////////////////////////////////////////////////////////

std::vector<std::string> reader_xtc::_supported_ext = {".xtc"};

///////////////////////////////////////////////////////////////////////////////

inline void reader_xtc::read_list(const std::string& home, const std::string& trajlist_path, 
        std::vector<float>& data, int& n_atoms)
{
    std::vector<std::string> trajlist;
    int n_samples = 0;

    // Checks if file exists
    if(!bfs::is_regular_file(home+trajlist_path)) FATAL_ERROR(home+trajlist_path+": File not found :("); 
    reader_xtc::get_framefile_list(trajlist, home, trajlist_path);

    data.clear(); // clears data

    // Number of atoms in the first file is number of atoms in all the files
    if(trajlist.size() <= 0) FATAL_ERROR("File list empty :("); 
    read_xtc_natoms((char*)trajlist[0].c_str(), &n_atoms);

    // Foreach file in trajlist
    for(std::string trajfile : trajlist)
    {
        DBG_MESSAGE("Reading file: " + trajfile + " ... "); // Debug Message
        reader_xtc::read_trajfile(trajfile, data, n_atoms, n_samples);
        DBG_MESSAGE(std::to_string(n_samples) + " frames found\n"); // Debug Message
    }
}

///////////////////////////////////////////////////////////////////////////////

inline bool reader_xtc::is_ext_supported(const std::string& str)
{
    for(std::string l : reader_xtc::_supported_ext)
    {
        if(!str.compare(str.size()-4, 4, l))
            return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////

inline void reader_xtc::get_framefile_list(std::vector<std::string>& trajlist,
        const std::string& home, const std::string& trajlinks_path)
{
    std::stringstream trajlinks; 
    std::string sub_traj;

    // Recursion base, when file is a complete path
    if(reader_xtc::is_ext_supported(trajlinks_path)) 
        trajlist.push_back(home + trajlinks_path);
    // when file is a link to a file
    else {
        // Gets contents of the file 
        trajlinks << std::ifstream(home + trajlinks_path, std::ios_base::in).rdbuf();
        trajlinks >> sub_traj;

        while(trajlinks && !trajlinks.eof() && sub_traj.size() > 0) // Foreach link in the file
        {
            if(bfs::is_regular_file(home+sub_traj)) // if file is openable and is a link to other files
                reader_xtc::get_framefile_list(trajlist, home, sub_traj);
            else if(bfs::is_regular_file(home+sub_traj+".xtc")) // if file without xtc extension
                trajlist.push_back(home+sub_traj+".xtc");
            else  // default file name
                trajlist.push_back(home+sub_traj+"/frame0.xtc");

            trajlinks >> sub_traj; // gets new file/link
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void reader_xtc::read_trajfile(const std::string& trajfile, 
        std::vector<float> &data, int& n_atoms, int& n_samples)
{
    int step;
    float time, prec;
    matrix box;
    rvec* tmp = new rvec[n_atoms];
    n_samples = 0;

    // Opens trajfile
    XDRFILE *xdr_file = xdrfile_open(trajfile.c_str(), "r"); // opens file

    // Reads each frame
    while (exdrOK == read_xtc(xdr_file, n_atoms, &step, &time, box, tmp, &prec)) {
        for(int i=0; i < n_atoms; i++) {
            data.push_back(tmp[i][0]);
            data.push_back(tmp[i][1]);
            data.push_back(tmp[i][2]);
        }
        n_samples++;
    }

    // Closes trajfile
    xdrfile_close(xdr_file);

    // Deletes allocated memory
    delete []tmp;
}


///////////////////////////////////////////////////////////////////////////////

#endif /* !READER_XTC_HPP */

///////////////////////////////////////////////////////////////////////////////

