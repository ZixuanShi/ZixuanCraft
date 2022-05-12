// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GPConflict.h"
#include "Auth/GPPermissionFlags.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"

#include "GPCloudSaveLibrary.generated.h"

UENUM()
enum class EConflictResolutionPolicy : uint8
{
	/** In the case of a conflict, the result will be returned to the app for resolution.*/
	Manual,

	/** In the case of a conflict, the snapshot with the longest played time will be used.*/
	LongestPlaytime,

	/** In the case of a conflict, the last known good version of this snapshot will be used.*/
	LastKnownGood,

	/** In the case of a conflict, the snapshot with the highest progress value will be used.*/
	HighestProgress,

	/** In the case of a conflict, the most recently modified version of this snapshot will be used.*/
	MostRecentlyModified
};

UCLASS()
class MOBILEHELPER_API UGPCloudSaveLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE(FGPDelegate);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnErrorDelegate, const FString&, ErrorMessage);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnDeleteSnapshotDelegate, const FString&, DeleteMessage);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnSnapshotMetadataDelegate, const UGPSnapshotMetadata*, SnapshotMetadata);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnSnapshotOpenSuccessDelegate, const UGPSnapshot*, Snapshot);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnSnapshotReadSuccessDelegate, const TArray<uint8>&, ByteArray);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnGetMaxSizeDelegate, int, MaxSize);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnLoadSnapshotMetadataDelegate, const TArray<UGPSnapshotMetadata*>&, SnapshotMetadataArray);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGPOnConflictDelegate, const UGPConflict*, Conflict);

public:
	UGPCloudSaveLibrary() = default;

	/**
	* You can integrate the snapshots API wherever your game provides players with the option to save or restore their progress.
	* Your game might display such an option at designated save/restore points or allow players to save or restore progress at any time.
	* Once players select the save/restore option in your game, your game can optionally bring up a screen that prompts players to enter information for a new saved game or to select an existing saved game to restore.
	* To simplify your development, the snapshots API provides a default saved games selection user interface (UI) that you can use out-of-the-box. 
	* The saved games selection UI allows players to create a new saved game, view details about existing saved games, and load previous saved games.
	* @param Title - The title to display in the action bar of the returned Activity.
	* @param AllowAddButton - Whether or not to display a "create new snapshot" option in the selection UI.
	* @param AllowDelete - Whether or not to provide a delete overflow menu option for each snapshot in the selection UI.
	* @param MaxSnapshots - The maximum number of snapshots to display in the UI. Use DISPLAY_LIMIT_NONE to display all snapshots.
	* @param OnAddNewSnapshot - Called if you add new snapshot
	* @param OnViewSnapshotMetadata - Called if you selected snapshot
	* @param OnCancel - Called if exit from saved games UI
	* @param OnError - Called if show saved games is failure
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnAddNewSnapshot, OnViewSnapshotMetadata, OnCancel, OnError"))
	static void ShowSavedGamesUI(const FString& Title, bool AllowAddButton, bool AllowDelete, int MaxSnapshots, const FGPDelegate& OnAddNewSnapshot, const FGPOnSnapshotMetadataDelegate& OnViewSnapshotMetadata, const FGPDelegate& OnCancel, const FGPOnErrorDelegate& OnError);

	/**
	* Retrieve saved games for the currently signed-in player
	* @param FileName - The name of the snapshot file to open. Must be between 1 and 100 non-URL-reserved characters (a-z, A-Z, 0-9, or the symbols "-", ".", "_", or "~").
	* @param CreateIfNotFound - If true, the snapshot will be created if one cannot be found.
	* @param ConflictPolicy - The conflict resolution policy to use for this snapshot.
	* @param OnSnapshotOpenSuccess - Called if snapshot was successfully opened.
	* @param OnOpenSnapshotError - Called if snapshot opening was failed.
	* @param OnOpenSnapshotByFileNameConflict - Called if snapshot opening was conflicted.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnSnapshotOpenSuccess,OnOpenSnapshotError,OnOpenSnapshotByFileNameConflict"))
	static void OpenSnapshotWithFileName(const FString& FileName, bool CreateIfNotFound, EConflictResolutionPolicy ConflictPolicy, const FGPOnSnapshotOpenSuccessDelegate& OnSnapshotOpenSuccess, const FGPOnErrorDelegate& OnOpenSnapshotError, const FGPOnConflictDelegate& OnOpenSnapshotByFileNameConflict);

	/**
	* Retrieve saved games for the currently signed-in player
	* @param SnapshotMetadata - The metadata of the existing snapshot to load.
	* @param ConflictPolicy - The conflict resolution policy to use for this snapshot.
	* @param OnSnapshotOpenWithMetadataSuccess - Called if snapshot was successfully opened.
	* @param OnOpenSnapshotWithMetadataError - Called if snapshot opening was failed.
	* @param OnOpenSnapshotWithMetadataConflict - Called if snapshot opening was conflicted.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnSnapshotOpenWithMetadataSuccess,OnOpenSnapshotWithMetadataError,OnOpenSnapshotWithMetadataConflict"))
	static void OpenSnapshotWithMetadata(UGPSnapshotMetadata* SnapshotMetadata, EConflictResolutionPolicy ConflictPolicy, const FGPOnSnapshotOpenSuccessDelegate& OnSnapshotOpenWithMetadataSuccess, const FGPOnErrorDelegate& OnOpenSnapshotWithMetadataError, const FGPOnConflictDelegate& OnOpenSnapshotWithMetadataConflict);
	
	/**
	* Write the contents to a snapshot.
	* @param Snapshot - Snapshot to content.
	* @param Data - The data to write into snapshot.
	* @param CoverImage - Image to snapshot.
	* @param Description - Description of snapshot.
	* @param PlayedTimeMillis - Played time in millis.
	* @param ProgressValue - Progress value.
	* @param OnWriteSnapshotSuccess - Called if write snapshot is successful.
	* @param OnWriteSnapshotFailure - Called if write snapshots is failed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnWriteSnapshotSuccess, OnWriteSnapshotFailure"))
	static void WriteSnapshot(UGPSnapshot* Snapshot, TArray<uint8> Data, UTexture2D* CoverImage, const FString& Description, int64 PlayedTimeMillis,  int64 ProgressValue, const FGPOnSnapshotMetadataDelegate& OnWriteSnapshotSuccess, const FGPOnErrorDelegate& OnWriteSnapshotFailure);
	
	/**
	* Read the contents of a snapshot.
	* @param Snapshot - Snapshot to read.
	* @param OnSnapshotReadSuccess - Called if snapshot reading is successful.
	* @param OnReadSnapshotFailure - Called if snapshot reading is failed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnSnapshotReadSuccess, OnReadSnapshotFailure"))
	static void ReadSnapshot(UGPSnapshot* Snapshot, const FGPOnSnapshotReadSuccessDelegate& OnSnapshotReadSuccess, const FGPOnErrorDelegate& OnReadSnapshotFailure);

	/**
	* Method to get the maximum data size per snapshot cover image in bytes
	* @param OnGetMaxCoverImageSizeSuccess - Called if on get max cover image size is successful and return int MaxSize
	* @param OnGetMaxCoverImageFailure - Called if on get max cover image size is failed and return error message
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnGetMaxCoverImageSizeSuccess, OnGetMaxCoverImageFailure"))
	static void GetMaxCoverImageSize(const FGPOnGetMaxSizeDelegate& OnGetMaxCoverImageSizeSuccess, const FGPOnErrorDelegate& OnGetMaxCoverImageFailure);

	/**
	* Method to get the maximum data size per snapshot in bytes
	* @param OnGetMaxDataSizeSuccess - Called if on get max data size is successful and return int MaxSize
	* @param OnGetMaxDataFailure - Called if on get max cover image size is failed and return error message
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnGetMaxDataSizeSuccess, OnGetMaxDataFailure"))
	static void GetMaxDataSize(const FGPOnGetMaxSizeDelegate& OnGetMaxDataSizeSuccess, const FGPOnErrorDelegate& OnGetMaxDataFailure);
	
	/**
	* You can retrieve and display (in the game) a list of players who are friends with the current user
	* As a user, it is possible to control which games have access to the friends list.
	* When you retrieve the friends list, you must handle the case where permission is required.
	* This is all encapsulated in the API to make requesting access and subsequently using the friends list a straightforward task.
	* @param SnapshotMetadata - The metadata of the snapshot to delete.
	* @param OnDeleteSnapshotSuccess - Called if snapshot is successfully deleted.
	* @param OnDeleteSnapshotFailure - Called if delete snapshot is failed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnDeleteSnapshotSuccess, OnDeleteSnapshotFailure"))
	static void DeleteSnapshot(UGPSnapshotMetadata* SnapshotMetadata, const FGPOnDeleteSnapshotDelegate& OnDeleteSnapshotSuccess, const FGPOnErrorDelegate& OnDeleteSnapshotFailure);

	/**
	* Loads an annotated SnapshotMetadataBuffer that represents the snapshot data for the currently signed-in player.
	* @param ForceReload - If true, this call will clear any locally cached data and attempt to fetch the latest data from the server. This would commonly be used for something like a user-initiated refresh. Normally, this should be set to false to gain advantages of data caching.
	* @param OnLoadSnapshotMetadata - Called if load snapshot was successful.
	* @param OnLoadSnapshotMetadataFailure - Called if load snapshot was failed.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnLoadSnapshotMetadata, OnLoadSnapshotMetadataFailure"))
	static void LoadSnapshotMetadata(bool ForceReload, const FGPOnLoadSnapshotMetadataDelegate& OnLoadSnapshotMetadata, const FGPOnErrorDelegate& OnLoadSnapshotMetadataFailure);
	
	/**
	* When using the snapshots API in your game, it is possible for multiple devices to perform reads and writes on the same saved game.
	* In the event that a device temporarily loses its network connection and later reconnects, this might cause data conflicts whereby the saved game stored on a player's local device is out-of-sync with the remote version stored in Google's servers.
	* The snapshots API provides a conflict resolution mechanism that presents both sets of conflicting saved games at read-time and lets you implement a resolution strategy that is appropriate for your game.
	* @param ConflictId - The ID of the conflict to resolve. Must come from getConflictId().
	* @param Snapshot - The snapshot to use to resolve the conflict.
	* @param OnResolveConflictSuccess - Called if conflict is resolved.
	* @param OnResolveConflictFailure - Called if resolve conflict was failed.
	* @param OnResolveConflictConflict - Called if resolve conflict was conflicted.
	*/
	UFUNCTION(BlueprintCallable, Category = "Mobile Helper GooglePlayServices|SaveCloud", meta = (AutoCreateRefTerm = "OnResolveConflictSuccess, OnResolveConflictFailure,OnResolveConflictConflict"))
	static void ResolveConflict(const FString& ConflictId, UGPSnapshot* Snapshot, const FGPOnSnapshotOpenSuccessDelegate& OnResolveConflictSuccess, const FGPOnErrorDelegate& OnResolveConflictFailure, const FGPOnConflictDelegate& OnResolveConflictConflict);

	static FGPDelegate OnCloudSaveCanceledDelegate;
	static FGPDelegate OnAddNewSnapshotDelegate;

	static FGPOnErrorDelegate OnCloudSaveErrorDelegate;
	static FGPOnErrorDelegate OnOpenSnapshotWithFileNameErrorDelegate;
	static FGPOnErrorDelegate OnOpenSnapshotWithMetadataErrorDelegate;
	static FGPOnErrorDelegate OnWriteSnapshotFailureDelegate;
	static FGPOnErrorDelegate OnReadSnapshotFailureDelegate;
	static FGPOnErrorDelegate OnGetMaxCoverImageFailureDelegate;
	static FGPOnErrorDelegate OnGetMaxDataFailureDelegate;
	static FGPOnErrorDelegate OnDeleteSnapshotFailureDelegate;
	static FGPOnErrorDelegate OnLoadSnapshotMetadataFailureDelegate;
	static FGPOnErrorDelegate OnResolveConflictFailureDelegate;

	static FGPOnDeleteSnapshotDelegate OnDeleteSnapshotSuccessDelegate;

	static FGPOnSnapshotOpenSuccessDelegate OnSnapshotOpenWithFileNameSuccessDelegate;
	static FGPOnSnapshotOpenSuccessDelegate OnSnapshotOpenWithMetadataSuccessDelegate;
	static FGPOnSnapshotOpenSuccessDelegate OnResolveConflictSuccessDelegate;

	static FGPOnSnapshotMetadataDelegate OnViewSnapshotMetadataDelegate;
	static FGPOnSnapshotMetadataDelegate OnWriteSnapshotSuccessDelegate;

	static FGPOnSnapshotReadSuccessDelegate OnSnapshotReadSuccessDelegate;

	static FGPOnGetMaxSizeDelegate OnGetMaxCoverImageSizeSuccessDelegate;
	static FGPOnGetMaxSizeDelegate OnGetMaxDataSizeSuccessDelegate;

	static FGPOnLoadSnapshotMetadataDelegate OnLoadSnapshotMetadataDelegate;

	static FGPOnConflictDelegate OnOpenSnapshotByFileNameConflictDelegate;
	static FGPOnConflictDelegate OnOpenSnapshotWithMetadataConflictDelegate;
	static FGPOnConflictDelegate OnResolveConflictConflictDelegate;

private:
	static int GetResolutionPolicy(EConflictResolutionPolicy ResolutionPolicy);
};