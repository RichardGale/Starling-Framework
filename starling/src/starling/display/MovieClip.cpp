// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "MovieClip.h"
#include "flash/errors/IllegalOperationError.h"
#include "flash/media/Sound.h"

#include "starling/animation/IAnimatable.h"
#include "starling/events/Event.h"
#include "starling/textures/Texture.h"

/** Dispatched whenever the movie has displayed its last frame. */























using namespace flash::errors;
using namespace flash::media;
using namespace starling::animation;
using namespace starling::events;
using namespace starling::textures;

namespace starling
{
    namespace display
    {
        /** A MovieClip is a simple way to display an animation depicted by a list of textures.
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
         */

        /** Creates a movie clip from the provided textures and with the specified default framerate.
         *  The movie will have the size of the first frame. */
        MovieClip::MovieClip(std::vector<Texture *> *textures, float fps)
        {
            if (textures->length> 0)
            {
                super(textures[0]);
                init(textures, fps);
            }
            else
            {
                throw new ArgumentError("Empty texture array");
            }
        }

        void MovieClip::init(std::vector<Texture *> *textures, float fps)
        {
            if (fps <= 0) throw new ArgumentError("Invalid fps: " + fps);
            int numFrames= textures->length;

            mDefaultFrameDuration = 1.0 / fps;
            mLoop = true;
            mPlaying = true;
            mCurrentTime = 0.0;
            mCurrentFrame = 0;
            mTextures = textures->concat();
            mSounds.clear();
            mDurations.clear();
            mStartTimes.clear();

            for ( int i=0; i<numFrames; ++i)
            {
                mDurations[i] = mDefaultFrameDuration;
                mStartTimes[i] = i * mDefaultFrameDuration;
            }
        }

        // frame manipulation

        /** Adds an additional frame, optionally with a sound and a custom duration. If the
         *  duration is omitted, the default framerate is used (as specified in the constructor). */
        void MovieClip::addFrame(Texture *texture, Sound *sound, float duration)
        {
            addFrameAt(numFrames, texture, sound, duration);
        }

        /** Adds a frame at a certain index, optionally with a sound and a custom duration. */
        void MovieClip::addFrameAt(int frameID, Texture *texture, Sound *sound,
                                   float duration)
        {
            if (frameID < 0 || frameID > numFrames) throw new ArgumentError("Invalid frame id");
            if (duration < 0) duration = mDefaultFrameDuration;

            mTextures.splice(frameID, 0, texture);
            mSounds.splice(frameID, 0, sound);
            mDurations.splice(frameID, 0, duration);

            if (frameID > 0 && frameID == numFrames)
                mStartTimes[frameID] = mStartTimes[int(frameID-1)] + mDurations[int(frameID-1)];
            else
                updateStartTimes();
        }

        /** Removes the frame at a certain ID. The successors will move down. */
        void MovieClip::removeFrameAt(int frameID)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            if (numFrames == 1) throw new IllegalOperationError("Movie clip must not be empty");

            mTextures.splice(frameID, 1);
            mSounds.splice(frameID, 1);
            mDurations.splice(frameID, 1);

            updateStartTimes();
        }

        /** Returns the texture of a certain frame. */
        Texture *MovieClip::getFrameTexture(int frameID)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            return mTextures[frameID];
        }

        /** Sets the texture of a certain frame. */
        void MovieClip::setFrameTexture(int frameID, Texture *texture)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            mTextures[frameID] = texture;
        }

        /** Returns the sound of a certain frame. */
        Sound *MovieClip::getFrameSound(int frameID)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            return mSounds[frameID];
        }

        /** Sets the sound of a certain frame. The sound will be played whenever the frame
         *  is displayed. */
        void MovieClip::setFrameSound(int frameID, Sound *sound)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            mSounds[frameID] = sound;
        }

        /** Returns the duration of a certain frame (in seconds). */
        float MovieClip::getFrameDuration(int frameID)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            return mDurations[frameID];
        }

        /** Sets the duration of a certain frame (in seconds). */
        void MovieClip::setFrameDuration(int frameID, float duration)
        {
            if (frameID < 0 || frameID >= numFrames) throw new ArgumentError("Invalid frame id");
            mDurations[frameID] = duration;
            updateStartTimes();
        }

        // playback methods

        /** Starts playback. Beware that the clip has to be added to a juggler, too! */
        void MovieClip::play()
        {
            mPlaying = true;
        }

        /** Pauses playback. */
        void MovieClip::pause()
        {
            mPlaying = false;
        }

        /** Stops playback, resetting "currentFrame" to zero. */
        void MovieClip::stop()
        {
            mPlaying = false;
            currentFrame = 0;
        }

        // helpers

        void MovieClip::updateStartTimes()
        {
            int numFrames= this->numFrames;

            mStartTimes.clear()
            mStartTimes[0] = 0;

            for ( int i=1; i<numFrames; ++i)
                mStartTimes[i] = mStartTimes[int(i-1)] + mDurations[int(i-1)];
        }

        // IAnimatable

        /** @inheritDoc */
        void MovieClip::advanceTime(float passedTime)
        {
            if (!mPlaying || passedTime <= 0.0) return;

            int finalFrame;
            int previousFrame= mCurrentFrame;
            float restTime = 0.0;
            bool breakAfterFrame   = false;
            bool hasCompleteListener   = hasEventListener(Event::COMPLETE);
            bool dispatchCompleteEvent   = false;
            float totalTime = this->totalTime;

            if (mLoop && mCurrentTime >= totalTime)
            {
                mCurrentTime = 0.0;
                mCurrentFrame = 0;
            }

            if (mCurrentTime < totalTime)
            {
                mCurrentTime += passedTime;
                finalFrame = mTextures.length - 1;

                while (mCurrentTime > mStartTimes[mCurrentFrame] + mDurations[mCurrentFrame])
                {
                    if (mCurrentFrame == finalFrame)
                    {
                        if (mLoop && !hasCompleteListener)
                        {
                            mCurrentTime -= totalTime;
                            mCurrentFrame = 0;
                        }
                        else
                        {
                            breakAfterFrame = true;
                            restTime = mCurrentTime - totalTime;
                            dispatchCompleteEvent = hasCompleteListener;
                            mCurrentFrame = finalFrame;
                            mCurrentTime = totalTime;
                        }
                    }
                    else
                    {
                        mCurrentFrame++;
                    }

                    Sound *sound=mSounds[mCurrentFrame];
                    if (sound) sound->play();
                    if (breakAfterFrame) break;
                }

                // special case when we reach *exactly* the total time.
                if (mCurrentFrame == finalFrame && mCurrentTime == totalTime)
                    dispatchCompleteEvent = hasCompleteListener;
            }

            if (mCurrentFrame != previousFrame)
                texture = mTextures[mCurrentFrame];

            if (dispatchCompleteEvent)
                dispatchEventWith(Event::COMPLETE);

            if (mLoop && restTime > 0.0)
                advanceTime(restTime);
        }

        /** Indicates if a (non-looping) movie has come to its end. */
        bool MovieClip::isComplete()
        {
            return !mLoop && mCurrentTime >= totalTime;
        }

        // properties

        /** The total duration of the clip in seconds. */
        float MovieClip::totalTime()
        {
            int numFrames= mTextures.length;
            return mStartTimes[int(numFrames-1)] + mDurations[int(numFrames-1)];
        }

        /** The time that has passed since the clip was started (each loop starts at zero). */
        float MovieClip::currentTime()
        {
            return mCurrentTime;
        }

        /** The total number of frames. */
        int MovieClip::numFrames()
        {
            return mTextures.length;
        }

        /** Indicates if the clip should loop. */
        bool MovieClip::loop()
        {
            return mLoop;
        }
        void MovieClip::loop(bool value)
        {
            mLoop = value;
        }

        /** The index of the frame that is currently displayed. */
        int MovieClip::currentFrame()
        {
            return mCurrentFrame;
        }
        void MovieClip::currentFrame(int value)
        {
            mCurrentFrame = value;
            mCurrentTime = 0.0;

            for ( int i=0; i<value; ++i)
                mCurrentTime += getFrameDuration(i);

            texture = mTextures[mCurrentFrame];
            if (mSounds[mCurrentFrame]) mSounds[mCurrentFrame]->play();
        }

        /** The default number of frames per second. Individual frames can have different
         *  durations. If you change the fps, the durations of all frames will be scaled
         *  relatively to the previous value. */
        float MovieClip::fps()
        {
            return 1.0 / mDefaultFrameDuration;
        }
        void MovieClip::fps(float value)
        {
            if (value <= 0) throw new ArgumentError("Invalid fps: " + value);

            float newFrameDuration = 1.0 / value;
            float acceleration = newFrameDuration / mDefaultFrameDuration;
            mCurrentTime *= acceleration;
            mDefaultFrameDuration = newFrameDuration;

            for ( int i=0; i<numFrames; ++i)
            {
                float duration = mDurations[i] * acceleration;
                mDurations[i] = duration;
            }

            updateStartTimes();
        }

        /** Indicates if the clip is still playing. Returns <code>false</code> when the end
         *  is reached. */
        bool MovieClip::isPlaying()
        {
            if (mPlaying)
                return mLoop || mCurrentTime < totalTime;
            else
                return false;
        }
    }
}

