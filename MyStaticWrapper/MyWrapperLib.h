#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fmod.hpp>
#include <fmod_errors.h>

namespace FmodStaticLib
{
	struct Vector3
	{
		Vector3(float xIn, float yIn, float zIn) : x(xIn), y(yIn), z(zIn) {};
		~Vector3() {};
		float x;
		float y;
		float z;
	};

	class MyWrapperLib
	{
	public:
		
		FMOD_VECTOR Vector3ToFmodVec(const Vector3& position)
		{
			FMOD_VECTOR fmod_vec;
			fmod_vec.x = position.x;
			fmod_vec.y = position.y;
			fmod_vec.z = position.z;

			return fmod_vec;
		}
		float dB_to_volume(float dB) {
			return powf(10.0f, 0.05f*dB);
		}
		float volume_to_dB(float volume) {
			return 20.0f *log10f(volume);
		}

		typedef std::map<std::string, FMOD::Sound*> SoundMap;
		typedef std::map<int, FMOD::Channel*> ChannelMap;
		typedef std::map<int, FMOD::Sound*> PlayedSoundsMap;


		MyWrapperLib();
		~MyWrapperLib();

		FMOD_RESULT LoadSound(const std::string, bool, bool, bool );
		FMOD_RESULT UnloadSound(const std::string&);
		FMOD_RESULT LoadSoundOnEmptyChannel(const std::string& strSoundName, const Vector3& vPosition, float volumedB, bool, bool, bool);
		bool SoundWasLoaded(const std::string&);
		bool ChannelIsUsed(int);
		FMOD_RESULT ChangeOptsOfLoadedSound(const std::string&, bool, bool, bool);
		FMOD_RESULT  GetOptsOfLoadedSound(const std::string&, bool*, bool*, bool*);
		
		FMOD_RESULT PlayChannel(int);
		FMOD_RESULT PlayAllChannels();

		FMOD_RESULT StopChannel(int);
		FMOD_RESULT StopAllChannels();

		FMOD_RESULT PauseAllChannels();

		std::wstring& DisplayChannelsState();

		void IncreaseChannelVolume(int);
		void DecreaseChannelVolume(int);
		float channelVolume;
		void CenterChannelPan(int);
		void MoveChannelPanRight(int);
		void MoveChannelPanLeft(int);
		float channelPanLevel = 0.0f;
		std::vector<float>* channelPanLevels = new std::vector<float>();

	private:
		FMOD::Sound* mySoundPtr = nullptr;
		FMOD_CREATESOUNDEXINFO mySoundInfo;
		bool playingChannelState;
		
		int nextChannel;
		SoundMap mySounds;
		ChannelMap myChannels;
		PlayedSoundsMap mySoundsOnChannels;
		
		//std::vector<ChannelMap::iterator> stoppedChannels;
		bool bChannelIsPlaying;
		unsigned int iChannelPosition;
		float chanPositionInSec;
		std::wstring outputString;

		FMOD::System * mySystem;
		FMOD::ChannelGroup* myMasterChannelGroup;
		FMOD_RESULT myResult;


	};

}