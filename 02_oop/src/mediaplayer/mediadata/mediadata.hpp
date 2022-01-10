#ifndef MEDIADATA_HPP
#define MEDIADATA_HPP

#include <string>
#include <memory>

namespace mediaplayer {
	
	constexpr char* MEDIA_FILE_FORMATS_NAME[] {"mp3", "mp4"}; 
	constexpr char FILTER_FILE_FORMAT[] {"*.mp3 *.mp4"};
	constexpr char FILTER_LIB_FORMAT[] {"*.mpllist"};
	#ifdef WIN32
		constexpr char SEARCH_DIRECTORY[] {"C:/Users/User"};
	#endif //WIN32
	#ifdef __linux__
		constexpr char SEARCH_DIRECTORY[] {"/home/student"};
	#endif // __linux__
	
	static const char* getFiltersFormat() { return FILTER_FILE_FORMAT; }
	static const char* getFiltersFormatLibs() { return FILTER_LIB_FORMAT; }
	static const char* getHomeDirectory() { return SEARCH_DIRECTORY; }
	
	enum MediaTypes : uint8_t {
		MP3 = 0, 
		MP4,
		UNDEFINE_TYPE
	};
	
	const char* getExtension(MediaTypes type);
	
	class MediaFile {
	private:
		MediaTypes mType;
		std::string mPath;
		std::string mName;
	protected:
		explicit MediaFile(const std::string& path, MediaTypes type = MediaTypes::UNDEFINE_TYPE);
	public:
		MediaTypes type() const { return mType; }
		std::string& path() { return mPath; }
		std::string& name() { return mName; }
		const char* extension() { return getExtension(mType); }
	public:
		virtual ~MediaFile() {}
	};
	
	class MP3File : public MediaFile {
	friend std::shared_ptr<MediaFile> facoryMediaFile(const std::string& path);
	protected:
		explicit MP3File(const std::string& path) : MediaFile(path, MediaTypes::MP3) { }
	};
	
	class MP4File : public MediaFile {
	friend std::shared_ptr<MediaFile> facoryMediaFile(const std::string& path);
	protected:
		explicit MP4File(const std::string& path) : MediaFile(path, MediaTypes::MP4) { }
	};
	
	std::shared_ptr<MediaFile> facoryMediaFile(const std::string& path);
}

#endif // MEDIADATA_HPP
