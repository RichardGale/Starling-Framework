#if !defined(__STARLING_SRC_STARLING_DISPLAY_MOVIECLIP_AS)
#define __STARLING_SRC_STARLING_DISPLAY_MOVIECLIP_AS
#if defined(__cplusplus)
// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include <map>
#include <string>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Math.h"
#include "Class.h"
#include "RegExp.h"
namespace flash { namespace errors { class IllegalOperationError; } }
namespace flash { namespace media { class Sound; } }

#include "starling/animation/IAnimatable.h"
namespace starling { namespace events { class Event; } }
namespace starling { namespace textures { class Texture; } }

    /** Dispatched whenever the movie has displayed its last frame. */
    //[Event(name="complete",type="starling.events.Event")]






















using namespace flash::errors;
using namespace flash::media;
using namespace starling::animation;
using namespace starling::events;
using namespace starling::textures;

namespace starling {
namespace display {
    class/** A MovieClip is a simple way to display an animation depicted by a list of textures.
     *  
     *  <p>Pass the frames of the movie in a vector of textures to the constructor. The movie clip 
     *  will have the width and height of the first frame. If you group your frames with the help 
     *  of a texture atlas (which is recommended), use the <code>getTextures</code>-method of the 
     *  atlas to receive the textures in the correct (alphabetic) order.</p> 
     *  
     *  <p>You can specify the desired framerate via the constructor. You can, however, manually 
     *  give each frame a custom duration. You can also play a sound whenever a certain frame 
     *  appears.</p>
     *  
     *  <p>The methods <code>play</code> and <code>pause</code> control playback of the movie. You
     *  will receive an event of type <code>Event.MovieCompleted</code> when the movie finished
     *  playback. If the movie is looping, the event is dispatched once per loop.</p>
     *  
     *  <p>As any animated object, a movie clip has to be added to a juggler (or have its 
     *  <code>advanceTime</code> method called regularly) to run. The movie will dispatch 
     *  an event of type "Event.COMPLETE" whenever it has displayed its last frame.</p>
     *  
     *  @see starling.textures.TextureAtlas
     */          MovieClip: public Image, public IAnimatable{private:  std::vector<Texture*>* mTextures;
        private:  std::vector<Sound*>* mSounds;
        private:  std::vector<float>* mDurations;
        private:  std::vector<float>* mStartTimes;

        private:  float mDefaultFrameDuration;
        private:  float mCurrentTime;
        private:  int mCurrentFrame;
        private:  bool mLoop;
        private:  bool mPlaying;

        /** Creates a movie clip from the provided textures and with the specified default framerate.
         *  The movie will have the size of the first frame. */
        public:          MovieClip(std::vector<Texture*>* textures, float fps =12);

        private: void     init(std::vector<Texture*>* textures, float fps);

        // frame manipulation

        /** Adds an additional frame, optionally with a sound and a custom duration. If the 
         *  duration is omitted, the default framerate is used (as specified in the constructor). */
        public: void     addFrame(Texture* texture, Sound* sound=NULL, float duration =-1);

        /** Adds a frame at a certain index, optionally with a sound and a custom duration. */
        public: void     addFrameAt(int frameID, Texture* texture, Sound* sound=NULL,
                                   float duration =-1);

        /** Removes the frame at a certain ID. The successors will move down. */
        public: void     removeFrameAt(int frameID);

        /** Returns the texture of a certain frame. */
        public: Texture* getFrameTexture(int frameID);

        /** Sets the texture of a certain frame. */
        public: void     setFrameTexture(int frameID, Texture* texture);

        /** Returns the sound of a certain frame. */
        public: Sound*   getFrameSound(int frameID);

        /** Sets the sound of a certain frame. The sound will be played whenever the frame 
         *  is displayed. */
        public: void     setFrameSound(int frameID, Sound* sound);

        /** Returns the duration of a certain frame (in seconds). */
        public: float    getFrameDuration(int frameID);

        /** Sets the duration of a certain frame (in seconds). */
        public: void     setFrameDuration(int frameID, float duration);

        // playback methods

        /** Starts playback. Beware that the clip has to be added to a juggler, too! */
        public: void     play();

        /** Pauses playback. */
        public: void     pause();

        /** Stops playback, resetting "currentFrame" to zero. */
        public: void     stop();

        // helpers

        private: void     updateStartTimes();

        // IAnimatable

        /** @inheritDoc */
        public: void     advanceTime(float passedTime);

        /** Indicates if a (non-looping) movie has come to its end. */
        public: bool         isComplete();

        // properties  

        /** The total duration of the clip in seconds. */
        public: float        totalTime();

        /** The time that has passed since the clip was started (each loop starts at zero). */
        public: float        currentTime();

        /** The total number of frames. */
        public: int          numFrames();

        /** Indicates if the clip should loop. */
        public: bool         loop();
        public: void         loop(bool value);

        /** The index of the frame that is currently displayed. */
        public: int          currentFrame();
        public: void         currentFrame(int value);

        /** The default number of frames per second. Individual frames can have different 
         *  durations. If you change the fps, the durations of all frames will be scaled 
         *  relatively to the previous value. */
        public: float        fps();
        public: void         fps(float value);

        /** Indicates if the clip is still playing. Returns <code>false</code> when the end 
         *  is reached. */
        public: bool         isPlaying();
    };
}
}

#endif // __STARLING_SRC_STARLING_DISPLAY_MOVIECLIP_AS
#endif // __cplusplus

