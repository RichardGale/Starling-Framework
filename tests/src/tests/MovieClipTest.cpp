// =================================================================================================
//
//  Starling Framework
//  Copyright 2011 Gamua OG. All Rights Reserved.
//
//  This program is free software. You can redistribute and/or modify it
//  in accordance with the terms of the accompanying license agreement.
//
// =================================================================================================



#include "MovieClipTest.h"
#include "flash/display3D/Context3DTextureFormat.h"

#include "flexunit/framework/Assert.h"

#include "org/flexunit/assertThat.h"
#include "org/flexunit/asserts/assertEquals.h"
#include "org/flexunit/asserts/assertFalse.h"
#include "org/hamcrest/number/closeTo.h"

#include "starling/display/MovieClip.h"
#include "starling/events/Event.h"
#include "starling/textures/ConcreteTexture.h"
#include "starling/textures/Texture.h"

using namespace flash::display3D;
using namespace flexunit::framework;
using namespace org::flexunit;
using namespace org::flexunit::asserts;
using namespace org::flexunit::asserts;
using namespace org::hamcrest::number;
using namespace starling::display;
using namespace starling::events;
using namespace starling::textures;
using namespace starling::textures;

namespace tests {



        void MovieClipTest::                testFrameManipulation()
        {
             float fps = 4.0;
             float frameDuration = 1.0 / fps;
             std::string format=Context3DTextureFormat::BGRA;

             Texture* texture0=new ConcreteTexture(NULL, format, 16, 16, false, false);
             Texture* texture1=new ConcreteTexture(NULL, format, 16, 16, false, false);
             Texture* texture2=new ConcreteTexture(NULL, format, 16, 16, false, false);
             Texture* texture3=new ConcreteTexture(NULL, format, 16, 16, false, false);

             MovieClip* movie=new MovieClip(new <Texture*>[texture0],fps);

            assertThat(movie->width,closeTo(texture0->width,E));
            assertThat(movie->height,closeTo(texture0->height,E));
            assertThat(movie->totalTime,closeTo(frameDuration, E));
            Assert::assertEquals(1,movie->numFrames);
            Assert::assertEquals(0,movie->currentFrame);
            Assert::assertEquals(true,movie->loop);
            Assert::assertEquals(true,movie->isPlaying);

            movie->pause();
            Assert::assertFalse(movie->isPlaying);

            movie->play();
            Assert::assertTrue(movie->isPlaying);

            movie->addFrame(texture1);
            Assert::assertEquals(2,movie->numFrames);
            Assert::assertEquals(texture0,movie->getFrameTexture(0));
            Assert::assertEquals(texture1,movie->getFrameTexture(1));
            Assert::assertNull(movie->getFrameSound(0));
            Assert::assertNull(movie->getFrameSound(1));
            assertThat(movie->getFrameDuration(0),closeTo(frameDuration, E));
            assertThat(movie->getFrameDuration(1),closeTo(frameDuration, E));

            movie->addFrame(texture2,NULL, 0.5);
            assertThat(movie->getFrameDuration(2),closeTo(0.5, E));
            assertThat(movie->totalTime,closeTo(1.0, E));

            movie->addFrameAt(2,texture3); // -> 0, 1, 3, 2
            Assert::assertEquals(4,movie->numFrames);
            Assert::assertEquals(texture1,movie->getFrameTexture(1));
            Assert::assertEquals(texture3,movie->getFrameTexture(2));
            Assert::assertEquals(texture2,movie->getFrameTexture(3));
            assertThat(movie->totalTime,closeTo(1.0 + frameDuration, E));

            movie->removeFrameAt(0);// -> 1, 3, 2
            Assert::assertEquals(3,movie->numFrames);
            Assert::assertEquals(texture1,movie->getFrameTexture(0));
            assertThat(movie->totalTime,closeTo(1.0, E));

            movie->removeFrameAt(1);// -> 1, 2
            Assert::assertEquals(2,movie->numFrames);
            Assert::assertEquals(texture1,movie->getFrameTexture(0));
            Assert::assertEquals(texture2,movie->getFrameTexture(1));
            assertThat(movie->totalTime,closeTo(0.75, E));

            movie->setFrameTexture(1,texture3);
            Assert::assertEquals(texture3,movie->getFrameTexture(1));

            movie->setFrameDuration(1,0.75);
            assertThat(movie->totalTime,closeTo(1.0, E));

            movie->addFrameAt(2,texture3);
            Assert::assertEquals(texture3,movie->getFrameTexture(2));
        }


        void MovieClipTest::                testAdvanceTime()
        {
             float fps = 4.0;
             float frameDuration = 1.0 / fps;
             std::string format=Context3DTextureFormat::BGRA;

             Texture* texture0=new ConcreteTexture(NULL, format, 16, 16, false, false);
             Texture* texture1=new ConcreteTexture(NULL, format, 16, 16, false, false);
             Texture* texture2=new ConcreteTexture(NULL, format, 16, 16, false, false);
             Texture* texture3=new ConcreteTexture(NULL, format, 16, 16, false, false);

             MovieClip* movie=new MovieClip(new <Texture*>[texture0],fps);
            movie->addFrame(texture2,NULL, 0.5);
            movie->addFrame(texture3);
            movie->addFrameAt(0,texture1);

            Assert::assertEquals(0,movie->currentFrame);
            movie->advanceTime(frameDuration/ 2.0);
            Assert::assertEquals(0,movie->currentFrame);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(1,movie->currentFrame);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(2,movie->currentFrame);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(2,movie->currentFrame);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(3,movie->currentFrame);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(0,movie->currentFrame);
            Assert::assertFalse(movie->isComplete);

            movie->loop= false;
            movie->advanceTime(movie->totalTime+frameDuration);
            Assert::assertEquals(3,movie->currentFrame);
            Assert::assertFalse(movie->isPlaying);
            Assert::assertTrue(movie->isComplete);

            movie->currentFrame= 0;
            Assert::assertEquals(0,movie->currentFrame);
            movie->advanceTime(frameDuration* 1.1);
            Assert::assertEquals(1,movie->currentFrame);

            movie->stop();
            Assert::assertFalse(movie->isPlaying);
            Assert::assertFalse(movie->isComplete);
            Assert::assertEquals(0,movie->currentFrame);
        }


        void MovieClipTest::                testChangeFps()
        {
             std::vector<Texture*>* frames=createFrames(3);
             MovieClip* movie=new MovieClip(frames, 4.0);

            assertThat(movie->fps,closeTo(4.0, E));

            movie->fps= 3.0;
            assertThat(movie->fps,closeTo(3.0, E));
            assertThat(movie->getFrameDuration(0),closeTo(1.0 / 3.0, E));
            assertThat(movie->getFrameDuration(1),closeTo(1.0 / 3.0, E));
            assertThat(movie->getFrameDuration(2),closeTo(1.0 / 3.0, E));

            movie->setFrameDuration(1,1.0);
            assertThat(movie->getFrameDuration(1),closeTo(1.0, E));

            movie->fps= 6.0;
            assertThat(movie->getFrameDuration(1),closeTo(0.5, E));
            assertThat(movie->getFrameDuration(0),closeTo(1.0 / 6.0, E));
        }


        void MovieClipTest::                testCompletedEvent()
        {
             float fps = 4.0;
             float frameDuration = 1.0 / fps;
             int completedCount= 0;

             std::vector<Texture*>* frames=createFrames(4);
             MovieClip* movie=new MovieClip(frames, fps);
            movie->addEventListener(Event::COMPLETE,onMovieCompleted);
            movie->loop= false;

            Assert::assertFalse(movie->isComplete);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(0,movie->currentFrame);
            Assert::assertEquals(0,completedCount);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(1,movie->currentFrame);
            Assert::assertEquals(0,completedCount);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(2,movie->currentFrame);
            Assert::assertEquals(0,completedCount);
            movie->advanceTime(frameDuration* 0.5);
            movie->advanceTime(frameDuration* 0.5);
            Assert::assertEquals(3,movie->currentFrame);
            Assert::assertEquals(1,completedCount);
            Assert::assertTrue(movie->isComplete);
            movie->advanceTime(movie->numFrames*2 * frameDuration);
            Assert::assertEquals(3,movie->currentFrame);
            Assert::assertEquals(1,completedCount);
            Assert::assertTrue(movie->isComplete);

            movie->loop= true;
            completedCount = 0;

            Assert::assertFalse(movie->isComplete);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(0,movie->currentFrame);
            Assert::assertEquals(0,completedCount);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(1,movie->currentFrame);
            Assert::assertEquals(0,completedCount);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(2,movie->currentFrame);
            Assert::assertEquals(0,completedCount);
            movie->advanceTime(frameDuration);
            Assert::assertEquals(3,movie->currentFrame);
            Assert::assertEquals(1,completedCount);
            movie->advanceTime(movie->numFrames*2 * frameDuration);
            Assert::assertEquals(3,completedCount);

            void MovieClipTest::(Event* event)
            {
                completedCount++;
            }
        }


        void MovieClipTest::                testChangeCurrentFrameInCompletedEvent()
        {
             float fps = 4.0;
             float frameDuration = 1.0 / fps;
             int completedCount= 0;

             std::vector<Texture*>* frames=createFrames(4);
             MovieClip* movie=new MovieClip(frames, fps);

            movie->loop= true;
            movie->addEventListener(Event::COMPLETE,onMovieCompleted);
            movie->advanceTime(1.75);

            Assert::assertFalse(movie->isPlaying);
            Assert::assertEquals(0,movie->currentFrame);

            void MovieClipTest::(Event* event)
            {
                movie->pause();
                movie->currentFrame= 0;
            }
        }


        void MovieClipTest::                testRemoveAllFrames()
        {
             std::vector<Texture*>* frames=createFrames(2);
             MovieClip* movie=new MovieClip(frames);

            // it must not be allowed to remove the last frame 
            movie->removeFrameAt(0);
             bool throwsError   = false;

            try
            {
                movie->removeFrameAt(0);
            }
            catch (error:Error*)
            {
                throwsError = true;
            }

            Assert::assertTrue(throwsError);
        }


        void MovieClipTest::                testLastTextureInFastPlayback()
        {
             float fps = 20.0;
             std::vector<Texture*>* frames=createFrames(3);
             MovieClip* movie=new MovieClip(frames, fps);
            movie->addEventListener(Event::COMPLETE,onMovieCompleted);
            movie->advanceTime(1.0);

            void MovieClipTest::()
            {
                Assert::assertEquals(frames[2],movie->texture);
            }
        }


        void MovieClipTest::                testAssignedTextureWithCompleteHandler()
        {
            // https://github.com/PrimaryFeather/Starling-Framework/issues/232

             std::vector<Texture*>* frames=createFrames(2);
             MovieClip* movie=new MovieClip(frames, 2);

            movie->addEventListener(Event::COMPLETE,onComplete);
            assertEquals(frames[0], movie->texture);

            movie->advanceTime(0.5);
            assertEquals(frames[0], movie->texture);

            movie->advanceTime(0.5);
            assertEquals(frames[1], movie->texture);

            movie->advanceTime(0.5);
            assertEquals(frames[0], movie->texture);

            void MovieClipTest::()      { /* does not have to do anything */ }
        }


        void MovieClipTest::                testStopMovieInCompleteHandler()
        {
             std::vector<Texture*>* frames=createFrames(5);
             MovieClip* movie=new MovieClip(frames, 5);

            movie->addEventListener(Event::COMPLETE,onComplete);
            movie->advanceTime(1.3);

            assertFalse(movie->isPlaying);
            assertThat(movie->currentTime,closeTo(0.0, E));
            assertEquals(frames[0], movie->texture);

            movie->play();
            movie->advanceTime(0.3);
            assertThat(movie->currentTime,closeTo(0.3, E));
            assertEquals(frames[1], movie->texture);

            void MovieClipTest::()      { movie->stop();}
        }

        std::vector<Texture*>* MovieClipTest::createFrames(int count)
        {
             std::vector<Texture*>* frames=new<Texture*>[];
             std::string format=Context3DTextureFormat::BGRA;

            for ( int i=0;i<count; ++i)
                frames.push_back(newConcreteTexture(NULL,format,16,16,false, false));

            return frames;
        }
}

