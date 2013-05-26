#if !defined(__STARLING_SRC_STARLING_UTILS_ASSETMANAGER_AS)
#define __STARLING_SRC_STARLING_UTILS_ASSETMANAGER_AS
#if defined(__cplusplus)


#include "flex11.6.h"
namespace flash
{
    namespace display
    {
        class Bitmap;
    }
}
namespace flash
{
    namespace display
    {
        class Loader;
    }
}
namespace flash
{
    namespace events
    {
        class Event;
    }
}
namespace flash
{
    namespace events
    {
        class IOErrorEvent;
    }
}
namespace flash
{
    namespace events
    {
        class ProgressEvent;
    }
}
namespace flash
{
    namespace media
    {
        class Sound;
    }
}
namespace flash
{
    namespace media
    {
        class SoundChannel;
    }
}
namespace flash
{
    namespace media
    {
        class SoundTransform;
    }
}
namespace flash
{
    namespace net
    {
        class FileReference;
    }
}
namespace flash
{
    namespace net
    {
        class URLLoader;
    }
}
namespace flash
{
    namespace net
    {
        class URLLoaderDataFormat;
    }
}
namespace flash
{
    namespace net
    {
        class URLRequest;
    }
}
namespace flash
{
    namespace system
    {
        class ImageDecodingPolicy;
    }
}
namespace flash
{
    namespace system
    {
        class LoaderContext;
    }
}
namespace flash
{
    namespace utils
    {
        class ByteArray;
    }
}
namespace flash
{
    namespace utils
    {
        class Dictionary;
    }
}
namespace flash
{
    namespace utils
    {
        class clearTimeout;
    }
}
namespace flash
{
    namespace utils
    {
        class describeType;
    }
}
namespace flash
{
    namespace utils
    {
        class getQualifiedClassName;
    }
}
namespace flash
{
    namespace utils
    {
        class setTimeout;
    }
}

namespace starling
{
    namespace core
    {
        class Starling;
    }
}
namespace starling
{
    namespace text
    {
        class BitmapFont;
    }
}
namespace starling
{
    namespace text
    {
        class TextField;
    }
}
namespace starling
{
    namespace textures
    {
        class Texture;
    }
}
namespace starling
{
    namespace textures
    {
        class TextureAtlas;
    }
}

/** The AssetManager handles loading and accessing a variety of asset types. You can
 *  add assets directly (via the 'add...' methods) or asynchronously via a queue. This allows
 *  you to deal with assets in a unified way, no matter if they are loaded from a file,
 *  directory, URL, or from an embedded object.
 *
 *  <p>If you load files from disk, the following types are supported:
 *  <code>png, jpg, atf, mp3, xml, fnt</code></p>
 */

using namespace flash::display;
using namespace flash::events;
using namespace flash::media;
using namespace flash::net;
using namespace flash::system;
using namespace flash::utils;
using namespace starling::core;
using namespace starling::text;
using namespace starling::textures;

namespace starling
{
    namespace utils
    {
        class AssetManager
        {
        private:
            const std::vector<std::string> SUPPORTED_EXTENSIONS;

        private:
            float mScaleFactor;
        private:
            bool mUseMipMaps;
        private:
            bool mCheckPolicyFile;
        private:
            bool mVerbose;

        private:
            std::vector<void *> mRawAssets;
        private:
            std::map<std::string, void *> mTextures;
        private:
            std::map<std::string, void *> mAtlases;
        private:
            std::map<std::string, void *> mSounds;

            /** helper objects */
        private:
            std::vector<std::string> sNames;

            /** Create a new AssetManager. The 'scaleFactor' and 'useMipmaps' parameters define
             *  how enqueued bitmaps will be converted to textures. */
        public:
            AssetManager(float scaleFactor=1, bool useMipmaps=false);

            /** Disposes all contained textures. */
        public:
            void     dispose();

            // retrieving

            /** Returns a texture with a certain name. The method first looks through the directly
             *  added textures; if no texture with that name is found, it scans through all
             *  texture atlases. */
        public:
            Texture  *getTexture(std::string name);

            /** Returns all textures that start with a certain string, sorted alphabetically
             *  (especially useful for "MovieClip"). */
        public:
            std::vector<Texture *> getTextures(std::string prefix="", std::vector<Texture *> result=std::vector<void *>());

            /** Returns all texture names that start with a certain string, sorted alphabetically. */
        public:
            std::vector<std::string> getTextureNames(std::string prefix="", std::vector<std::string> result=std::vector<std::string>());

            /** Returns a texture atlas with a certain name, or null if it's not found. */
        public:
            TextureAtlas *getTextureAtlas(std::string name);

            /** Returns a sound with a certain name. */
        public:
            Sound    *getSound(std::string name);

            /** Returns all sound names that start with a certain string, sorted alphabetically. */
        public:
            std::vector<std::string> getSoundNames(std::string prefix="");

            /** Generates a new SoundChannel object to play back the sound. This method returns a
             *  SoundChannel object, which you can access to stop the sound and to control volume. */
        public:
            SoundChannel *playSound(std::string name, float startTime=0, int loops=0,
                                    SoundTransform *transform=NULL);

            // direct adding

            /** Register a texture under a certain name. It will be available right away. */
        public:
            void     addTexture(std::string name, Texture *texture);

            /** Register a texture atlas under a certain name. It will be available right away. */
        public:
            void     addTextureAtlas(std::string name, TextureAtlas *atlas);

            /** Register a sound under a certain name. It will be available right away. */
        public:
            void     addSound(std::string name, Sound *sound);

            // removing

            /** Removes a certain texture, optionally disposing it. */
        public:
            void     removeTexture(std::string name, bool dispose=true);

            /** Removes a certain texture atlas, optionally disposing it. */
        public:
            void     removeTextureAtlas(std::string name, bool dispose=true);

            /** Removes a certain sound. */
        public:
            void     removeSound(std::string name);

            /** Removes assets of all types and empties the queue. */
        public:
            void     purge();

            // queued adding

            /** Enqueues one or more raw assets; they will only be available after successfully
             *  executing the "loadQueue" method. This method accepts a variety of different objects:
             *
             *  <ul>
             *    <li>Strings containing an URL to a local or remote resource. Supported types:
             *        <code>png, jpg, atf, mp3, fnt, xml</code> (texture atlas).</li>
             *    <li>Instances of the File class (AIR only) pointing to a directory or a file.
             *        Directories will be scanned recursively for all supported types.</li>
             *    <li>Classes that contain <code>static</code> embedded assets.</li>
             *  </ul>
             *
             *  Suitable object names are extracted automatically: A file named "image.png" will be
             *  accessible under the name "image". When enqueuing embedded assets via a class,
             *  the variable name of the embedded object will be used as its name. An exception
             *  are texture atlases: they will have the same name as the actual texture they are
             *  referencing.
             */
        public:
            void     enqueue(...);

            /** Enqueues a single asset with a custom name that can be used to access it later.
             *  If you don't pass a name, it's attempted to generate it automatically.
             *  @returns the name under which the asset was registered. */
        public:
            std::string   enqueueWithName(Object *asset, std::string name="");

            /** Loads all enqueued assets asynchronously. The 'onProgress' function will be called
             *  with a 'ratio' between '0.0' and '1.0', with '1.0' meaning that it's complete.
             *
             *  @param onProgress: <code>function(ratio:Number):void;</code>
             */
        public:
            void     loadQueue(Function *onProgress);

        private:
            void     loadRawAsset(std::string name, Object *rawAsset, std::vector<XML *> xmls,
                                  Function *onProgress, Function *onComplete);

            // helpers

            /** This method is called by 'enqueue' to determine the name under which an asset will be
             *  accessible; override it if you need a custom naming scheme. Typically, 'rawAsset' is
             *  either a String or a FileReference. Note that this method won't be called for embedded
             *  assets. */
        protected:
            std::string   getName(Object *rawAsset);

            /** This method is called during loading of assets when 'verbose' is activated. Per
             *  default, it traces 'message' to the console. */
        protected:
            void     log(std::string message);

            /** This method is called during loading of assets when a bitmap texture is processed.
             *  Override it if you want to preprocess the bitmap in some way. */
        protected:
            void     addBitmapTexture(std::string name, Bitmap *bitmap);

            // properties

            /** When activated, the class will trace information about added/enqueued assets. */
        public:
            bool      verbose();
        public:
            void         verbose(bool value);

            /** For bitmap textures, this flag indicates if mip maps should be generated when they
             *  are loaded; for ATF textures, it indicates if mip maps are valid and should be
             *  used. */
        public:
            bool      useMipMaps();
        public:
            void         useMipMaps(bool value);

            /** Textures that are created from Bitmaps or ATF files will have the scale factor
             *  assigned here. */
        public:
            float       scaleFactor();
        public:
            void         scaleFactor(float value);

            /** Specifies whether a check should be made for the existence of a URL policy file before
             *  loading an object from a remote server. More information about this topic can be found
             *  in the 'flash.system.LoaderContext' documentation. */
        public:
            bool      checkPolicyFile();
        public:
            void         checkPolicyFile(bool value);
        };
    }
}

#endif // __STARLING_SRC_STARLING_UTILS_ASSETMANAGER_AS
#endif // __cplusplus

