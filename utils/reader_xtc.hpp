/*
 * ============================================================================
 *       Filename:  reader_xtc.hpp
 *    Description:  
 *        Created:  2015-03-30 18:49
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * ============================================================================
 */

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

#define MAX_FILE_NAME_LENGTH 128

///////////////////////////////////////////////////////////////////////////////

namespace bfs = boost::filesystem;

///////////////////////////////////////////////////////////////////////////////

class ReaderXTC
{
    public:
        static inline void read_list(const std::string& home, const std::string& trajlist,
                std::vector<float>& data, int& n_atoms);

    protected:
        static inline void read_file(const std::string& trajfile, std::vector<float>&
                data, int& n_atoms, int& n_samples);

        /**
         * Given a trajlist path, this function will recursively try to find
         * the (*.xtc) files and insert it on the framefile_list. The complete
         * path for the file must be given by home/trajlist 
         * */
        static inline void get_framefile_list(std::vector<std::string>& framefile_list,
                const std::string& home, const std::string& trajlist);

        static inline void renormalize(std::vector<float>& data, int n_atoms, 
                int n_samples);

        /**
         * Checks if the extension of file "file_name" is supported or not
         * by this class
         * */
        static inline bool is_ext_supported(const std::string& file_name);

    private:
        static XDRFILE* _xdr_file;

        static std::vector<std::string> _supported_ext;
};

///////////////////////////////////////////////////////////////////////////////

XDRFILE* ReaderXTC::_xdr_file;

std::vector<std::string> ReaderXTC::_supported_ext = {".xtc"};

///////////////////////////////////////////////////////////////////////////////

inline void ReaderXTC::read_list(const std::string& home, const std::string& trajlist_path, 
        std::vector<float>& data, int& n_atoms)
{
    std::vector<std::string> trajlist;
    int n_samples = 0;

    // Checks if file exists
    if(!bfs::is_regular_file(home+trajlist_path)) FATAL_ERROR(home+trajlist_path+": File not found :("); 
    ReaderXTC::get_framefile_list(trajlist, home, trajlist_path);

    data.clear(); // clears data

    // Number of atoms in the first file is number of atoms in all the files
    if(trajlist.size() <= 0) FATAL_ERROR("File list empty :("); 
    read_xtc_natoms((char*)trajlist[0].c_str(), &n_atoms);

    // Foreach file in trajlist
    for(std::string trajfile : trajlist)
    {
        std::cout << "Reading file: " << trajfile << " ... ";
        ReaderXTC::read_file(trajfile, data, n_atoms, n_samples);
        std::cout << n_samples << " frames found" << std::endl;
    }

    //ReaderXTC::renormalize(data, n_atoms, n_samples);
}

///////////////////////////////////////////////////////////////////////////////

inline bool ReaderXTC::is_ext_supported(const std::string& str)
{
    for(std::string l : ReaderXTC::_supported_ext)
    {
        if(!str.compare(str.size()-4, 4, l))
            return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////

inline void ReaderXTC::get_framefile_list(std::vector<std::string>& trajlist,
        const std::string& home, const std::string& trajlinks_path)
{
    // Recursion base, when file is a complete path
    if(ReaderXTC::is_ext_supported(trajlinks_path)) 
        trajlist.push_back(home + trajlinks_path);
    // when file is a link to a file
    else {
        std::stringstream trajlinks; 
        std::string sub_traj;

        trajlinks << std::ifstream(home + trajlinks_path, std::ios_base::in).rdbuf();
        trajlinks >> sub_traj;

        while(trajlinks && !trajlinks.eof() && sub_traj.size() > 0) // Foreach link in the file
        {
            //if(std::ifstream(home+sub_traj).good()) // if file is openable and is a link to other files
            if(bfs::is_regular_file(home+sub_traj)) // if file is openable and is a link to other files
                ReaderXTC::get_framefile_list(trajlist, home, sub_traj);
            else if(bfs::is_regular_file(home+sub_traj+".xtc")) // if file without xtc extension
                trajlist.push_back(home+sub_traj+".xtc");
            else  // default file name
                trajlist.push_back(home+sub_traj+"/frame0.xtc");

            trajlinks >> sub_traj;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

inline void ReaderXTC::read_file(const std::string& trajfile, 
        std::vector<float> &data, int& n_atoms, int& n_samples)
{
    int step;
    float time, prec;
    matrix box;
    rvec* tmp = new rvec[n_atoms];
    n_samples = 0;

    ReaderXTC::_xdr_file = xdrfile_open(trajfile.c_str(), "r"); // opens file

    // Count number of frames
    while (exdrOK == read_xtc(ReaderXTC::_xdr_file, n_atoms, &step, &time, 
                box, tmp, &prec)) {
        for(int i=0; i < n_atoms; i++) {
            data.push_back(tmp[i][0]);
            data.push_back(tmp[i][1]);
            data.push_back(tmp[i][2]);
        }
        n_samples++;
    }

    xdrfile_close(ReaderXTC::_xdr_file);

    delete []tmp;
}


///////////////////////////////////////////////////////////////////////////////

inline void swap_small(std::vector<float>& data, int i, int j)
{
    float tmp = data[i];

    data[i] = data[j];
    data[j] = tmp;
}

inline void swap(std::vector<float>& data, int i, int j)
{
    register int ii = 3*i;
    register int jj = 3*j;

    swap_small(data, ii, jj);
    swap_small(data, ii+1, jj+1);
    swap_small(data, ii+2, jj+2);
}

inline void ReaderXTC::renormalize(std::vector<float>& data, int n_atoms, 
        int n_samples)
{
    for(int i=0; i < n_samples; i++) 
    {
        for(int j=i+1; j < n_atoms; j++)
        {
            swap(data, i*n_atoms + j,  + n_atoms * j);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

#endif /* !READER_XTC_HPP */

///////////////////////////////////////////////////////////////////////////////

