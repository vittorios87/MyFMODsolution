#include "stdafx.h"
#include "MyWrapperLib.h"

namespace FmodStaticLib
{
	using namespace FMOD;
	using namespace std;
	MyWrapperLib::MyWrapperLib()
	{

		//System * mySystem;
		//FMOD_RESULT myResult;
		myResult = System_Create(&mySystem);
		if (myResult != FMOD_OK)
		{
			cout << "FMOD error:  " << myResult << "  " << FMOD_ErrorString(myResult) << endl;
			exit(-1);
		}
		myResult = mySystem->init(8, FMOD_INIT_NORMAL, NULL);
		if (myResult != FMOD_OK)
		{
			cout << "FMOD error:  " << myResult << "  " << FMOD_ErrorString(myResult) << endl;
			exit(-1);
		}
		myResult = mySystem->getMasterChannelGroup(&myMasterChannelGroup);
		//string nameChannelGroup{ "my channels" };
		//myResult = mySystem->createChannelGroup(nameChannelGroup.c_str(), &myChannelGroup);
		cout << FMOD_ErrorString(myResult) << endl;
	}

	FMOD_RESULT MyWrapperLib::LoadSound(const std::string strSoundName,bool b3d, bool bLooping, bool bStream)
	{
		auto foundIterator = mySounds.find(strSoundName);
		if (foundIterator != mySounds.end())
		{
			return FMOD_OK;
		}
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3d ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		FMOD::Sound*soundPtr = nullptr;
		//FMOD_CREATESOUNDEXINFO* soundInfo= new FMOD_CREATESOUNDEXINFO();
		myResult = mySystem->createSound(strSoundName.c_str(), eMode, 0, &soundPtr);
		if (soundPtr) {
			mySounds[strSoundName] = soundPtr;
		}
		
		return myResult;
	}
	bool MyWrapperLib::SoundWasLoaded(const string& strSoundName) {
		auto foundIterator = mySounds.find(strSoundName);
		if (foundIterator == mySounds.end())
		{
			return false;
		}
		return true;
	}
	bool MyWrapperLib::ChannelIsUsed(int chNmbr)
	{
		auto foundChIt = myChannels.find(chNmbr);
		if (foundChIt == myChannels.end())
		{
			return false;
		}
		return true;
	}

	FMOD_RESULT  MyWrapperLib::ChangeOptsOfLoadedSound(const string& strSoundName, bool b3d, bool bLooping, bool bStream)
	{
		auto foundIterator = mySounds.find(strSoundName);
		FMOD_MODE eMode = FMOD_DEFAULT;
		eMode |= b3d ? FMOD_3D : FMOD_2D;
		eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
		eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
		return foundIterator->second->setMode(eMode);
	}
	FMOD_RESULT  MyWrapperLib::GetOptsOfLoadedSound(const string& strSoundName, bool* b3d, bool* bLooping, bool* bStream)
	{
		auto foundIt = mySounds.find(strSoundName);
		FMOD_MODE eMode;
		foundIt->second->getMode(&eMode);
		*b3d = eMode & FMOD_3D;
		*bLooping = eMode & FMOD_LOOP_NORMAL;
		*bStream = eMode & FMOD_CREATESTREAM;
		return FMOD_OK;
	}


	FMOD_RESULT MyWrapperLib::LoadSoundOnEmptyChannel(const string& strSoundName,const Vector3& vPosition, float volumedB, bool b3d, bool bLooping, bool bStream) {

		auto foundIterator = mySounds.find(strSoundName);
		if (foundIterator == mySounds.end())
		{
			LoadSound(strSoundName, b3d, bLooping, bStream);
			foundIterator = mySounds.find(strSoundName);
			if (foundIterator == mySounds.end())
			{
				return FMOD_ERR_FILE_NOTFOUND;
			}
			
		}
		
		Channel* myChannel=nullptr;
		myResult = mySystem->playSound(foundIterator->second, myMasterChannelGroup, true, &myChannel);
		if (myChannel)
		{
			FMOD_MODE currentMode;
			myResult = foundIterator->second->getMode(&currentMode);
			if (currentMode & FMOD_3D)
			{
				FMOD_VECTOR position = Vector3ToFmodVec(vPosition);
				myResult = myChannel->set3DAttributes(&position, nullptr);
			}

			myResult = myChannel->setVolume(dB_to_volume(volumedB));
			myResult = myChannel->setPaused(false);

			myChannels.emplace(std::make_pair(nextChannel, myChannel));
			mySoundsOnChannels.emplace(std::make_pair(nextChannel, foundIterator->second));
			channelPanLevels->push_back(0.0f);
			++nextChannel;
		}

		return myResult;
	}
	FMOD_RESULT MyWrapperLib::UnloadSound(const std::string& strSoundName) {

		auto foundIterator = mySounds.find(strSoundName);
		if (foundIterator == mySounds.end())
		{
			return FMOD_ERR_FILE_NOTFOUND;
		}
		myResult=foundIterator->second->release();
		mySounds.erase(foundIterator);
		return myResult;
	}

	FMOD_RESULT MyWrapperLib::PlayChannel(int channelNumber)
	{
		auto foundChannelIt = myChannels.find(channelNumber);
		if (foundChannelIt != myChannels.end())
		{

			foundChannelIt->second->isPlaying(&playingChannelState);
			if (!playingChannelState)
			{				
				//foundChannelIt->second->getCurrentSound(&mySoundPtr);
				mySystem->playSound(mySoundsOnChannels[foundChannelIt->first], myMasterChannelGroup, false, &(foundChannelIt->second));
			}
		}
		return FMOD_OK;
	}
	FMOD_RESULT MyWrapperLib::PlayAllChannels()
	{
		for (auto it = myChannels.begin(), itEnd = myChannels.end(); it != itEnd; ++it)
		{
			it->second->isPlaying(&playingChannelState);
			if (!playingChannelState)
			{			
				mySystem->playSound(mySoundsOnChannels[it->first], myMasterChannelGroup, false, &(it->second));
			}
		}
		return FMOD_OK;
	}

	FMOD_RESULT MyWrapperLib::StopChannel(int channelNumber)
	{
		auto foundChannelIt = myChannels.find(channelNumber);
		if (foundChannelIt!=myChannels.end())
		{
			return foundChannelIt->second->stop();
		}
		return FMOD_OK;
	}
	FMOD_RESULT MyWrapperLib::StopAllChannels()
	{
		
		return myMasterChannelGroup->stop();
	}
	FMOD_RESULT MyWrapperLib::PauseAllChannels() 
	{
		bool paused;
		myMasterChannelGroup->getPaused(&paused);

		return myMasterChannelGroup->setPaused(!paused);
	}

	std::wstring& MyWrapperLib::DisplayChannelsState()
	{
		outputString.clear();
		outputString.append(L"                      CHANNELS IN USE\n\n");
		outputString.append(L"CHANNEL"); 
		outputString.append(L"\t\t");
		outputString.append(L"PLAY/STOP");
		outputString.append(L"\t");
		outputString.append(L"TIME");
		outputString.append(L"\t\t\t");
		outputString.append(L"VOLUME");
		outputString.append(L"\t\t\t");
		outputString.append(L"PAN");
		outputString.append(L"\n\n");

		for (auto it = myChannels.begin(), itEnd = myChannels.end(); it != itEnd; ++it)
		{
			bChannelIsPlaying = false;
			iChannelPosition = 0;
			it->second->isPlaying(&bChannelIsPlaying);
			it->second->getPosition(&iChannelPosition, FMOD_TIMEUNIT_MS);
			chanPositionInSec = iChannelPosition / 1000;
			it->second->getVolume(&channelVolume);
			channelVolume = volume_to_dB(channelVolume);
			outputString.append(L"channel " + std::to_wstring(it->first) 
				+ L"\t"+ std::to_wstring(bChannelIsPlaying) 
				+ L"\t\t" + std::to_wstring(chanPositionInSec) 
				+ L"\t\t" + std::to_wstring(channelVolume) 
				+ L"\t\t" + std::to_wstring(channelPanLevels->at(it->first)) + L"\n");

		}
		outputString.append(L"\n\n");
		return outputString;
	}

	void MyWrapperLib::IncreaseChannelVolume(int channel) {
		auto channelIt = myChannels.find(channel);
		if (channelIt != myChannels.end())
		{
			channelIt->second->getVolume(&channelVolume);
			channelVolume = volume_to_dB(channelVolume);
			channelVolume += 0.1f;
			channelVolume = dB_to_volume(channelVolume);
			channelIt->second->setVolume(channelVolume);
		}

	}

	void MyWrapperLib::DecreaseChannelVolume(int channel) {
		auto channelIt = myChannels.find(channel);
		if (channelIt != myChannels.end())
		{
			channelIt->second->getVolume(&channelVolume);
			channelVolume = volume_to_dB(channelVolume);
			channelVolume -= 0.1f;
			channelVolume = dB_to_volume(channelVolume);
			channelIt->second->setVolume(channelVolume);
		}
	}
	void MyWrapperLib::CenterChannelPan(int channel) {
		auto channelIt = myChannels.find(channel);
		if (channelIt != myChannels.end())
		{	
			(*channelPanLevels)[channel] = 0.0f;
			myResult = channelIt->second->setPan(0.0f);
		}
	}
	void MyWrapperLib::MoveChannelPanRight(int channel) {
		auto channelIt = myChannels.find(channel);
		if (channelIt != myChannels.end())
		{
			channelPanLevel = channelPanLevels->at(channel);
			channelPanLevel = (channelPanLevel += 0.2f) > 1.0f ? 1.0f : channelPanLevel;
			(*channelPanLevels)[channel] = channelPanLevel;
			myResult = channelIt->second->setPan(channelPanLevel);
		}
	}
	void MyWrapperLib::MoveChannelPanLeft(int channel) {
		auto channelIt = myChannels.find(channel);
		if (channelIt != myChannels.end())
		{
			channelPanLevel = channelPanLevels->at(channel);
			channelPanLevel = (channelPanLevel -= 0.2f) < -1.0f ? -1.0f : channelPanLevel;
			(*channelPanLevels)[channel] = channelPanLevel;
			myResult = channelIt->second->setPan(channelPanLevel);
		}
	}
	

	MyWrapperLib::~MyWrapperLib()
	{
		myMasterChannelGroup->release();
		
	}
}