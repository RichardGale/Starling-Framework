

#include "AssetManager.h"
#include "flash/display/Bitmap.h"
#include "flash/display/Loader.h"
#include "flash/events/Event.h"
#include "flash/events/IOErrorEvent.h"
#include "flash/events/ProgressEvent.h"
#include "flash/media/Sound.h"
#include "flash/media/SoundChannel.h"
#include "flash/media/SoundTransform.h"
#include "flash/net/FileReference.h"
#include "flash/net/URLLoader.h"
#include "flash/net/URLLoaderDataFormat.h"
#include "flash/net/URLRequest.h"
#include "flash/system/ImageDecodingPolicy.h"
#include "flash/system/LoaderContext.h"
#include "flash/utils/ByteArray.h"
#include "flash/utils/Dictionary.h"
#include "flash/utils/clearTimeout.h"
#include "flash/utils/describeType.h"
#include "flash/utils/getQualifiedClassName.h"
#include "flash/utils/setTimeout.h"

#include "starling/core/Starling.h"
#include "starling/text/BitmapFont.h"
#include "starling/text/TextField.h"
#include "starling/textures/Texture.h"
#include "starling/textures/TextureAtlas.h"

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

namespace starling {
namespace utils {


                      


                    
                    
                    
                    

                    
                    
                    
                    

        /** helper objects */
                    

        /** Create a new AssetManager. The 'scaleFactor' and 'useMipmaps' parameters define
         *  how enqueued bitmaps will be converted to textures. */
        AssetManager::AssetManager(float scaleFactor, bool useMipmaps)
        {
            mVerbose = false;
            mScaleFactor = scaleFactor > 0 ? scaleFactor : Starling::contentScaleFactor;
            mUseMipMaps = useMipmaps;
            mCheckPolicyFile = false;
            mRawAssets = [];
            mTextures.clear();
            mAtlases.clear();
            mSounds.clear();
        }

        /** Disposes all contained textures. */
        void AssetManager::dispose()
        {
            for (std::vector<Texture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
                texture->dispose();

            for (std::vector<TextureAtlas*>::iterator atlas = mAtlases.begin(); atlas != mAtlases.end(); ++atlas)
                atlas->dispose();
        }

        // retrieving

        /** Returns a texture with a certain name. The method first looks through the directly
         *  added textures; if no texture with that name is found, it scans through all 
         *  texture atlases. */
        Texture* AssetManager::getTexture(std::string name)
        {
            if (name in !mTextures.empty()) return mTextures[name];
            else
            {
                for (std::vector<TextureAtlas*>::iterator atlas = mAtlases.begin(); atlas != mAtlases.end(); ++atlas)
                {
                    Texture* texture = atlas->getTexture(name);
                    if (texture) return texture;
                }
                return NULL;
            }
        }

        /** Returns all textures that start with a certain string, sorted alphabetically
         *  (especially useful for "MovieClip"). */
        std::vector<Texture*> AssetManager::getTextures(std::string prefix, std::vector<Texture*> result)
        {
            if (result.empty()) result.clear();

            for (std::vector<std::string>::iterator name = .begin(); name != .end(); ++name)
                result.push_back(getTexture(name));

            sNames.clear()    ;
            return result;
        }

        /** Returns all texture names that start with a certain string, sorted alphabetically. */
        std::vector<std::string> AssetManager::getTextureNames(std::string prefix, std::vector<std::string> result)
        {
            if (result.empty()) result.clear();

            for(std::vector<std::string>::iterator name = mTextures.begin(); name != mTextures.end(); ++name)
                if (name.indexOf(prefix) == 0)
                    result.push_back(name);

            for (std::vector<TextureAtlas*>::iterator atlas = mAtlases.begin(); atlas != mAtlases.end(); ++atlas)
                atlas->getNames(prefix, result);

            result.sort(Array()->CASEINSENSITIVE());
            return result;
        }

        /** Returns a texture atlas with a certain name, or null if it's not found. */
        TextureAtlas* AssetManager::getTextureAtlas(std::string name)
        {
            return dynamic_cast<TextureAtlas*>(mAtlases[name]);
        }

        /** Returns a sound with a certain name. */
        Sound* AssetManager::getSound(std::string name)
        {
            return mSounds[name];
        }

        /** Returns all sound names that start with a certain string, sorted alphabetically. */
        std::vector<std::string> AssetManager::getSoundNames(std::string prefix)
        {
            std::vector<std::string> names=std::vector<std::string>()            ;

            for(std::vector<std::string>::iterator name = mSounds.begin(); name != mSounds.end(); ++name)
                if (name.indexOf(prefix) == 0)
                    names.push_back(name);

            return names.sort(Array()->CASEINSENSITIVE());
        }

        /** Generates a new SoundChannel object to play back the sound. This method returns a 
         *  SoundChannel object, which you can access to stop the sound and to control volume. */
        SoundChannel* AssetManager::playSound(std::string name, float startTime, int loops,
                                  SoundTransform* transform)
        {
            if (name in !mSounds.empty())
                return getSound(name)->play(startTime, loops, transform);
            else
                return NULL;
        }

        // direct adding

        /** Register a texture under a certain name. It will be available right away. */
        void AssetManager::addTexture(std::string name, Texture* texture)
        {
            log("Adding texture '" + name + "'");

            if (name in !mTextures.empty())
                throw new Error("Duplicate texture name: " + name);
            else
                mTextures[name] = texture;
        }

        /** Register a texture atlas under a certain name. It will be available right away. */
        void AssetManager::addTextureAtlas(std::string name, TextureAtlas* atlas)
        {
            log("Adding texture atlas '" + name + "'");

            if (name in !mAtlases.empty())
                throw new Error("Duplicate texture atlas name: " + name);
            else
                mAtlases[name] = atlas;
        }

        /** Register a sound under a certain name. It will be available right away. */
        void AssetManager::addSound(std::string name, Sound* sound)
        {
            log("Adding sound '" + name + "'");

            if (name in !mSounds.empty())
                throw new Error("Duplicate sound name: " + name);
            else
                mSounds[name] = sound;
        }

        // removing

        /** Removes a certain texture, optionally disposing it. */
        void AssetManager::removeTexture(std::string name, bool dispose)
        {
            if (dispose && name in mTextures)
                mTextures[name]->dispose();

            delete mTextures[name];
        }

        /** Removes a certain texture atlas, optionally disposing it. */
        void AssetManager::removeTextureAtlas(std::string name, bool dispose)
        {
            if (dispose && name in mAtlases)
                mAtlases[name]->dispose();

            delete mAtlases[name];
        }

        /** Removes a certain sound. */
        void AssetManager::removeSound(std::string name)
        {
            delete mSounds[name];
        }

        /** Removes assets of all types and empties the queue. */
        void AssetManager::purge()
        {
            for (std::vector<Texture*>::iterator texture = mTextures.begin(); texture != mTextures.end(); ++texture)
                texture->dispose();

            for (std::vector<TextureAtlas*>::iterator atlas = mAtlases.begin(); atlas != mAtlases.end(); ++atlas)
                atlas->dispose();

            mRawAssets.clear()    ;
            mTextures.clear();
            mAtlases.clear();
            mSounds.clear();
        }

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
        void AssetManager::enqueue(...)
        {
            for (std::vector<Object*>::iterator rawAsset = rawAssets.begin(); rawAsset != rawAssets.end(); ++rawAsset)
            {
                if (dynamic_cast<std::vector<void*>>(rawAsset))
                {
                    enqueue()->apply(this, rawAsset);
                }
                else if (dynamic_cast<Class*>(rawAsset))
                {
                    XML* typeXml = describeType(rawAsset);
                    XML* childNode;

                    if (mVerbose)
                        log("Looking for static embedded assets in '" +
                            (typeXml->@name())->split("::")->pop() + "'");

                    for (std::vector<XML*>::iterator childNode = ..begin(); childNode != ..end(); ++childNode)
                        enqueueWithName(rawAsset[childNode->@name()], childNode->@name());

                    for (std::vector<XML*>::iterator childNode = ..begin(); childNode != ..end(); ++childNode)
                        enqueueWithName(rawAsset[childNode->@name()], childNode->@name());
                }
                else if (getQualifiedClassName(rawAsset) == "flash.filesystem::File")
                {
                    if (!rawAsset["exists"])
                    {
                        log("File or directory not found: '" + rawAsset["url"] + "'");
                    }
                    else if (!rawAsset["isHidden"])
                    {
                        if (rawAsset["isDirectory"])
                            enqueue()->apply(this, rawAsset["getDirectoryListing"]());
                        else
                        {
                            std::string extension = rawAsset["extension"]->toLowerCase();
                            if (SUPPORTED_EXTENSIONS.indexOf(extension) != -1)
                                enqueueWithName(rawAsset["url"]);
                            else
                                log("Ignoring unsupported file '" + rawAsset["name"] + "'");
                        }
                    }
                }
                else if (dynamic_cast<std::string>(rawAsset))
                {
                    enqueueWithName(rawAsset);
                }
                else
                {
                    log("Ignoring unsupported asset type: " + getQualifiedClassName(rawAsset));
                }
            }
        }

        /** Enqueues a single asset with a custom name that can be used to access it later. 
         *  If you don't pass a name, it's attempted to generate it automatically.
         *  @returns the name under which the asset was registered. */
        std::string AssetManager::enqueueWithName(Object* asset, std::string name)
        {
            if (name == NULL) name = getName(asset);
            log("Enqueuing '" + name + "'");

            mRawAssets.push_back({
                name: name,
                asset: asset
            });

            return name;
        }

        /** Loads all enqueued assets asynchronously. The 'onProgress' function will be called
         *  with a 'ratio' between '0.0' and '1.0', with '1.0' meaning that it's complete.
         *
         *  @param onProgress: <code>function(ratio:Number):void;</code> 
         */
        void AssetManager::loadQueue(Function* onProgress)
        {
            if (Starling::context() == NULL)
                throw new Error("The Starling instance needs to be ready before textures can be loaded.");

            std::vector<XML*> xmls=std::vector<void*>()         ;
            int numElements = mRawAssets.size();
            float currentRatio = 0.0;
            unsigned int timeoutID;

            resume();void AssetManager::()
            {
                currentRatio = mRawAssets.length ? 1.0 - (mRawAssets.size() / numElements) : 1.0;

                if (mRawAssets.size())
                    timeoutID = setTimeout(processNext, 1);
                else
                    processXmls();

                if (onProgress != NULL)
                    onProgress(currentRatio);
            }void AssetManager::()
            {
                Object* assetInfo = mRawAssets.pop();
                clearTimeout(timeoutID);
                loadRawAsset(assetInfo->name(), assetInfo->asset(), xmls, progress, resume);
            }void AssetManager::()
            {
                // xmls are processed seperately at the end, because the textures they reference
                // have to be available for other XMLs. Texture atlases are processed first:
                // that way, their textures can be referenced, too.

                xmls.sort(function(XML* a, XML* b)int  {
                    return a->localName() == "TextureAtlas" ? -1 : 1;
                });

                for (std::vector<XML*>::iterator xml = xmls.begin(); xml != xmls.end(); ++xml)
                {
                    std::string name;
                    std::string rootNode = xml->localName();

                    if (rootNode == "TextureAtlas")
                    {
                        name = getName(xml->@imagePath()->toString());

                        Texture* atlasTexture = getTexture(name);
                        addTextureAtlas(name, new TextureAtlas(atlasTexture, xml));
                        removeTexture(name, false);
                    }
                    else if (rootNode == "font")
                    {
                        name = getName(xml->pages()->page()->@file()->toString());

                        Texture* fontTexture = getTexture(name);
                        TextField::registerBitmapFont(new BitmapFont(fontTexture, xml));
                        removeTexture(name, false);
                    }
                    else
                        throw new Error("XML contents not recognized: " + rootNode);
                }
            }void AssetManager::(float ratio)
            {
                onProgress(currentRatio + (1.0 / numElements) * Math::min(1.0, ratio) * 0.99);
            }
        }

        void AssetManager::loadRawAsset(std::string name, Object* rawAsset, std::vector<XML*> xmls,
                                      Function* onProgress, Function* onComplete)
        {
            std::string extension = "";

            if (dynamic_cast<Class*>(rawAsset))
            {
                Object* asset = new rawAsset();

                if (dynamic_cast<Sound*>(asset))
                {
                    addSound(name, dynamic_cast<Sound*>(asset));
                    onComplete();
                }
                else if (dynamic_cast<Bitmap*>(asset))
                {
                    addBitmapTexture(name, dynamic_cast<Bitmap*>(asset));
                    onComplete();
                }
                else if (dynamic_cast<ByteArray*>(asset))
                {
                    ByteArray* bytes = dynamic_cast<ByteArray*>(asset);
                    std::string signature = String()->fromCharCode(bytes[0], bytes[1], bytes[2]);

                    if (signature == "ATF")
                    {
                        addTexture(name, Texture::fromAtfData(dynamic_cast<ByteArray*>(asset), mScaleFactor,
                            mUseMipMaps, onComplete));
                    }
                    else
                    {
                        xmls.push_back(new XML(bytes));
                        onComplete();
                    }
                }
                else
                {
                    log("Ignoring unsupported asset type: " + getQualifiedClassName(asset));
                    onComplete();
                }
            }
            else if (dynamic_cast<std::string>(rawAsset))
            {
                std::string url = dynamic_cast<std::string>(rawAsset);
                extension = url.split(".")->pop()->toLowerCase()->split("?")[0];

                URLLoader* urlLoader = new URLLoader();
                urlLoader->dataFormat ( URLLoaderDataFormat::BINARY);
                urlLoader->addEventListener(IOErrorEvent::IO_ERROR, onIoError);
                urlLoader->addEventListener(ProgressEvent::PROGRESS, onLoadProgress);
                urlLoader->addEventListener(Event::COMPLETE, onUrlLoaderComplete);
                urlLoader->load(new URLRequest(url));
            }void AssetManager::(IOErrorEvent* event)
            {
                log("IO error: " + event->text());
                onComplete();
            }void AssetManager::(ProgressEvent* event)
            {
                onProgress(event->bytesLoaded() / event->bytesTotal());
            }void AssetManager::(Event* event)
            {
                URLLoader* urlLoader = event->dynamic_cast<URLLoader*>(target);
                ByteArray* bytes = urlLoader->dynamic_cast<ByteArray*>(data);
                Sound* sound;

                urlLoader->removeEventListener(IOErrorEvent::IO_ERROR, onIoError);
                urlLoader->removeEventListener(ProgressEvent::PROGRESS, onProgress);
                urlLoader->removeEventListener(Event::COMPLETE, onUrlLoaderComplete);

                switch (extension)
                {
                    case "atf":
                        addTexture(name, Texture::fromAtfData(bytes, mScaleFactor, mUseMipMaps, onComplete));
                        break;
                    case "fnt":
                    case "xml":
                        xmls.push_back(new XML(bytes));
                        onComplete();
                        break;
                    case "mp3":
                        sound = new Sound();
                        sound->loadCompressedDataFromByteArray(bytes, bytes->length());
                        addSound(name, sound);
                        onComplete();
                        break;
                    default:
                        LoaderContext* loaderContext = new LoaderContext(mCheckPolicyFile);
                        Loader* loader = new Loader();
                        loaderContext->imageDecodingPolicy ( ImageDecodingPolicy::ON_LOAD);
                        loader->contentLoaderInfo()->addEventListener(Event::COMPLETE, onLoaderComplete);
                        loader->loadBytes(urlLoader->dynamic_cast<ByteArray*>(data), loaderContext);
                        break;
                }
            }void AssetManager::(Event* event)
            {
                event->target()->removeEventListener(Event::COMPLETE, onLoaderComplete);
                Object* content = event->target()->content;

                if (dynamic_cast<Bitmap*>(content))
                    addBitmapTexture(name, dynamic_cast<Bitmap*>(content));
                else
                    throw new Error("Unsupported asset type: " + getQualifiedClassName(content));

                onComplete();
            }
        }

        // helpers

        /** This method is called by 'enqueue' to determine the name under which an asset will be
         *  accessible; override it if you need a custom naming scheme. Typically, 'rawAsset' is 
         *  either a String or a FileReference. Note that this method won't be called for embedded
         *  assets. */
        std::string AssetManager::getName(Object* rawAsset)
        {
            std::vector<void*> matches;
            std::string name;

            if (dynamic_cast<std::string>(rawAsset) || dynamic_cast<FileReference*>(rawAsset))
            {
                name = dynamic_cast<std::string>(rawAsset) ? dynamic_cast<std::string>(rawAsset) : (dynamic_cast<FileReference*>(rawAsset))->name;
                name = name.replace(/%20/g, " "); // URLs use '%20' for spaces
                matches = /(.*[\\\/])?(.+)(\.[\w]{1,4})/()->exec(name);

                if (matches && matches.size() == 4) return matches[2];
                else throw new ArgumentError("Could not extract name from String '" + rawAsset + "'");
            }
            else
            {
                name = getQualifiedClassName(rawAsset);
                throw new ArgumentError("Cannot extract names for objects of type '" + name + "'");
            }
        }

        /** This method is called during loading of assets when 'verbose' is activated. Per
         *  default, it traces 'message' to the console. */
        void AssetManager::log(std::string message)
        {
            if (mVerbose) trace("[AssetManager]", message);
        }

        /** This method is called during loading of assets when a bitmap texture is processed. 
         *  Override it if you want to preprocess the bitmap in some way. */
        void AssetManager::addBitmapTexture(std::string name, Bitmap* bitmap)
        {
            addTexture(name, Texture::fromBitmap(bitmap, mUseMipMaps, false, mScaleFactor));
        }

        // properties

        /** When activated, the class will trace information about added/enqueued assets. */
        bool AssetManager::verbose()         { return mVerbose; }
        void AssetManager::verbose(bool value)      { mVerbose = value; }

        /** For bitmap textures, this flag indicates if mip maps should be generated when they 
         *  are loaded; for ATF textures, it indicates if mip maps are valid and should be
         *  used. */
        bool AssetManager::useMipMaps()         { return mUseMipMaps; }
        void AssetManager::useMipMaps(bool value)      { mUseMipMaps = value; }

        /** Textures that are created from Bitmaps or ATF files will have the scale factor 
         *  assigned here. */
        float AssetManager::scaleFactor()        { return mScaleFactor; }
        void AssetManager::scaleFactor(float value)      { mScaleFactor = value; }

        /** Specifies whether a check should be made for the existence of a URL policy file before
         *  loading an object from a remote server. More information about this topic can be found 
         *  in the 'flash.system.LoaderContext' documentation. */
        bool AssetManager::checkPolicyFile()         { return mCheckPolicyFile; }
        void AssetManager::checkPolicyFile(bool value)      { mCheckPolicyFile = value; }
}
}

