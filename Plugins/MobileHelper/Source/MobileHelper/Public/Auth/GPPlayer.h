// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif

#include "GPPlayer.generated.h"

class UTexture2D;

UENUM()
enum class EBannerImageOrientation : uint8
{
	Portrait,
	Landscape
};

UENUM()
enum class EFriendsListVisibilityStatus : uint8
{
	/** Constant indicating that currently it's unknown if the friends list is visible to the game, or whether the game can ask for access from the user.*/
	Unknown,

	/** Constant indicating that the friends list is currently visible to the game.*/
	Visible,

	/** Constant indicating that the friends list is not visible to the game, but the game can ask for access.*/
	RequestRequired,

	/** Constant indicating that the friends list is currently unavailable for the game. */
	FeatureUnavailable
};

UENUM()
enum class EPlayerFriendStatus : uint8
{
	/** Constant indicating that the currently signed-in player's friend status with this player is unknown. */
	Unknown,

	/** Constant indicating that the currently signed-in player is not a friend of this player, and there are no pending invitations between them. */
	NoRelationship,

	/** Constant indicating that the currently signed-in player and this player are friends. */
	Friend
};

UCLASS(BlueprintType)
class MOBILEHELPER_API UGPPlayer : public UObject
{
	GENERATED_BODY()

public:
	UGPPlayer() = default;
	virtual ~UGPPlayer();

#if PLATFORM_ANDROID
	void Init(jobject PlayerObject);
#endif

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnImageReadyDelegate, UTexture2D*, ImageTexture);

	/**
	* Get the player's display name
	* @return - Player's display name
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Player")
	FString PlayerName() const;

	/**
	* Get the player's id
	* @return - Player's id
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Player")
	FString PlayerId() const;

	/**
	* Get the player's title
	* @return - Player's title
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Player")
	FString PlayerTitle() const;

	/** 
	* Retrieves the timestamp at which this player record was last updated locally. 	
	* @return - Player's timestamp at which this player record was last updated locally. 
	*/
	UFUNCTION(BlueprintPure, Category = "Mobile Helper GooglePlayServices|Player")
	FDateTime GetPlayerRetrievedTimestamp() const;

	/**
	* Get Friends List Visibility Status
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|Player")
	EFriendsListVisibilityStatus GetFriendsListVisibilityStatus() const;
	
	/**
	* Get Player Friend Status
	* 
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|Player")
	EPlayerFriendStatus GetPlayerFriendStatus() const;

	/**
	* Get the player's banner image
	* @param OnImageReady - Called when the image is ready for use
	* @param Orientation - Device orientation to determine the image size
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|Player")
	void GetBannerImage(const FGPOnImageReadyDelegate& OnImageReady, EBannerImageOrientation Orientation);

	/**
	* Get the player's icon image
	* @param OnImageReady - Called when the image is ready for use
	* @param bIsHiRes - Indicates if the requested image should be high resolution
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|Player")
	void GetIconImage(const FGPOnImageReadyDelegate& OnImageReady, bool bIsHiRes);

	void OnBannerImageReady(UTexture2D* Tex) { OnBannerImageReadyDelegate.ExecuteIfBound(Tex); }
	void OnIconImageReady(UTexture2D* Tex) { OnIconImageReadyDelegate.ExecuteIfBound(Tex); }

private:
	FGPOnImageReadyDelegate OnIconImageReadyDelegate;
	FGPOnImageReadyDelegate OnBannerImageReadyDelegate;

#if PLATFORM_ANDROID
	jobject Player;
	jmethodID GetDisplayNameMethod;
	jmethodID GetPlayerIdMethod;
	jmethodID GetTitleMethod;
	jmethodID GetBannerImageMethod;
	jmethodID GetIconImageMethod;
	jmethodID GetRetrievedTimestampMethod;
	jmethodID GetFriendsListVisibilityStatusMethod;
	jmethodID GetPlayerFriendStatusMethod;
#endif
};
