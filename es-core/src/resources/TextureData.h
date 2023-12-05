#pragma once
#ifndef ES_CORE_RESOURCES_TEXTURE_DATA_H
#define ES_CORE_RESOURCES_TEXTURE_DATA_H

#include <mutex>
#include <string>
#include <vector>
#include "ImageIO.h"

class TextureResource;

class IPdfHandler
{
public:
	virtual int getPdfPageCount(const std::string& fileName) = 0;
	virtual std::vector<std::string> extractPdfImages(const std::string& fileName, int pageIndex = -1, int pageCount = 1, int quality = 0) = 0;
};

class TextureData
{
public:
	static IPdfHandler* PdfHandler;

	TextureData(bool tile, bool linear);
	~TextureData();

	// These functions populate mDataRGBA but do not upload the texture to VRAM

	//!!!! Needs to be canonical path. Caller should check for duplicates before calling this
	void initFromPath(const std::string& path);
	bool initSVGFromMemory(const unsigned char* fileData, size_t length);
	bool initImageFromMemory(const unsigned char* fileData, size_t length, int subImageIndex = -1);
	bool initFromRGBA(unsigned char* dataRGBA, size_t width, size_t height, bool copyData = true);

	// Read the data into memory if necessary
	bool load(bool updateCache = false);
	bool loadFromCbz();
	bool loadFromPdf();
	bool loadFromVideo();

	bool isLoaded();

	// Upload the texture to VRAM if necessary and bind. Returns true if bound ok or
	// false if either not loaded
	bool uploadAndBind();

	// Release the texture from VRAM
	void releaseVRAM();

	// Release the texture from conventional RAM
	void releaseRAM();

	// Get the amount of VRAM currenty used by this texture
	inline size_t getEstimatedVRAMUsage() { return mWidth * mHeight * 4; }
	inline size_t getVRAMUsage() { return mTextureID != 0 || mDataRGBA != nullptr ? mWidth * mHeight * 4 : 0; }

	size_t width();
	size_t height();
	float sourceWidth();
	float sourceHeight();
	void setSourceSize(float width, float height);

	bool tiled() { return mTile; }

	unsigned char* getDataRGBA() {
		return mDataRGBA;
	}

	void setMaxSize(MaxSizeInfo maxSize);
	bool isMaxSizeValid();

	void setTemporarySize(float width, float height);

	inline const std::string& getPath() { return mPath; };

	bool updateFromExternalRGBA(unsigned char* dataRGBA, size_t width, size_t height);

	inline bool isRequired() { return mRequired; };
	void setRequired(bool value) { mRequired = value; };

	inline bool isDynamic() { return mDynamic; };
	void setDynamic(bool value) { mDynamic = value; };

	inline bool isReloadable() { return mReloadable; };

private:
	bool			mRequired;

	std::mutex		mMutex;
	bool			mTile;
	bool			mLinear;
	std::string		mPath;
	unsigned int	mTextureID;
	unsigned char*	mDataRGBA;
	size_t			mWidth;
	size_t			mHeight;
	float			mSourceWidth;
	float			mSourceHeight;
	bool			mScalable;
	bool			mReloadable;
	bool			mDynamic;

	MaxSizeInfo		mMaxSize;
	Vector2i		mPackedSize;
	Vector2i		mBaseSize;

	bool			mIsExternalDataRGBA;
};

#endif // ES_CORE_RESOURCES_TEXTURE_DATA_H
