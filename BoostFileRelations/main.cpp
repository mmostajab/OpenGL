#include <boost\filesystem\fstream.hpp>
#include <boost\filesystem.hpp>
#include <string>

using namespace std;
namespace fs = ::boost::filesystem;

// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
void get_all(const fs::path& root, const string& ext, vector<fs::path>& ret)
{
    if(!fs::exists(root) || !fs::is_directory(root)) return;

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;

    while(it != endit)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ext) ret.push_back(it->path());
        ++it;

    }

}

template < >
boost::filesystem::path& boost::filesystem::path::append< typename boost::filesystem::path::iterator >( typename boost::filesystem::path::iterator begin, typename boost::filesystem::path::iterator end, const codecvt_type& cvt)
{ 
    for( ; begin != end ; ++begin )
        *this /= *begin;
    return *this;
}

boost::filesystem::path make_relative( boost::filesystem::path a_From, boost::filesystem::path a_To )
{
    a_From = boost::filesystem::absolute( a_From ); a_To = boost::filesystem::absolute( a_To );
    boost::filesystem::path ret;
    boost::filesystem::path::const_iterator itrFrom( a_From.begin() ), itrTo( a_To.begin() );
    // Find common base
    for( boost::filesystem::path::const_iterator toEnd( a_To.end() ), fromEnd( a_From.end() ) ; itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo );
    // Navigate backwards in directory to reach previously found base
    for( boost::filesystem::path::const_iterator fromEnd( a_From.end() ); itrFrom != fromEnd; ++itrFrom )
    {
        if( (*itrFrom) != "." )
            ret /= "..";
    }
    // Now navigate down the directory branch
    ret.append( itrTo, a_To.end() );
    return ret;
}

int main(int argc, const char* argv[])
{
	vector<fs::path> allFiles;

	if(argc == 1)
	{
		get_all(boost::filesystem::current_path(), ".h", allFiles);
		get_all(boost::filesystem::current_path(), ".cpp", allFiles);
		get_all(boost::filesystem::current_path(), ".inl", allFiles);
	}
	else if(argc == 2)
	{
		get_all(argv[1], ".h", allFiles);
		get_all(argv[1], ".cpp", allFiles);
		get_all(argv[1], ".inl", allFiles);
	}
		
	std::ofstream output("filenames.txt");
	vector<fs::path>::iterator iter = allFiles.begin();
	for(; iter != allFiles.end(); iter++)
	{
		if((*iter).extension() == ".h")
		{
			if(argc == 1)
			{
				output << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
				std::cout << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
			}
			else if(argc == 2)
			{
				output << make_relative(argv[1], (*iter))	<< std::endl;
				std::cout << make_relative(argv[1], (*iter))	<< std::endl;
			}
		}

		if((*iter).extension() == ".inl")
		{
			if(argc == 1)
			{
				output << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
				std::cout << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
			}
			else if(argc == 2)
			{
				output << make_relative(argv[1], (*iter))	<< std::endl;
				std::cout << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
			}
		}

		if((*iter).extension() == ".cpp")
		{
			if(argc == 1)
			{
				output << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
				std::cout << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
			}
			else if(argc == 2)
			{
				output << make_relative(argv[1], (*iter))	<< std::endl;
				std::cout << make_relative(boost::filesystem::current_path(), (*iter))	<< std::endl;
			}
		}
	}
	
	return 0;
}