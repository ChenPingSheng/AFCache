/*
 *
 * Copyright 2008 Artifacts - Fine Software Development
 * http://www.artifacts.de
 * Author: Michael Markowski (m.markowski@artifacts.de)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */




@interface AFCacheableItemInfo : NSObject <NSCoding> {
	NSTimeInterval requestTimestamp;
	NSTimeInterval responseTimestamp;
	NSDate *serverDate;
	NSTimeInterval age;
	NSNumber *maxAge;
	NSDate *expireDate;
	NSDate *lastModified;
	NSString *eTag;
	int statusCode;
	uint64_t contentLength;
	NSString *mimeType;
	NSURL *responseURL;
}

@property (nonatomic, assign) NSTimeInterval requestTimestamp;
@property (nonatomic, assign) NSTimeInterval responseTimestamp;

@property (nonatomic, retain) NSDate *lastModified;
@property (nonatomic, retain) NSDate *serverDate;
@property (nonatomic, assign) NSTimeInterval age;
@property (nonatomic, copy) NSNumber *maxAge;
@property (nonatomic, retain) NSDate *expireDate;
@property (nonatomic, copy) NSString *eTag;
@property (nonatomic, assign) int statusCode;
@property (nonatomic, assign) uint64_t contentLength;
@property (nonatomic, copy) NSString *mimeType;

@property (nonatomic, retain) NSURL *responseURL; // may differ from url when redirection or URL rewriting has occured. nil if URL has not been modified.

@end/*
 *
 * Copyright 2008 Artifacts - Fine Software Development
 * http://www.artifacts.de
 * Author: Michael Markowski (m.markowski@artifacts.de)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifdef USE_TOUCHXML

#endif





#define kAFCacheExpireInfoDictionaryFilename @"kAFCacheExpireInfoDictionary"
#define kAFCachePackageInfoDictionaryFilename @"afcache_packageInfos"

#define LOG_AFCACHE(m) NSLog(m);

#define kAFCacheUserDataFolder @".userdata"

// max cache item size in bytes
#define kAFCacheDefaultMaxFileSize 1000000

// max number of concurrent connections 
#define kAFCacheDefaultConcurrentConnections 5

#define kHTTPHeaderIfModifiedSince @"If-Modified-Since"
#define kHTTPHeaderIfNoneMatch @"If-None-Match"

//do housekeeping every nth time archive is called (per session)
#define kHousekeepingInterval 10

#define kDefaultDiskCacheDisplacementTresholdSize 100000000

#define kDefaultNetworkTimeoutIntervalIMSRequest 45
#define kDefaultNetworkTimeoutIntervalGETRequest 100
#define kDefaultNetworkTimeoutIntervalPackageRequest 10

#define kAFCacheNSErrorDomain @"AFCache"

#define USE_ASSERTS true

extern const char* kAFCacheContentLengthFileAttribute;
extern const char* kAFCacheDownloadingFileAttribute;
extern const double kAFCacheInfiniteFileSize;

enum {
	kAFCacheInvalidateEntry         = 1 << 9,
	//	kAFCacheUseLocalMirror		= 2 << 9, deprecated, don't redefine id 2 for compatibility reasons
	//	kAFCacheLazyLoad			= 3 << 9, deprecated, don't redefine id 3 for compatibility reasons
	kAFIgnoreError                  = 1 << 11,
    kAFCacheIsPackageArchive        = 1 << 12,
	kAFCacheRevalidateEntry         = 1 << 13, // revalidate even when cache is switched to offline
};

typedef struct NetworkTimeoutIntervals {
	NSTimeInterval IMSRequest;
	NSTimeInterval GETRequest;
	NSTimeInterval PackageRequest;
} NetworkTimeoutIntervals;

@class AFCache;
@class AFCacheableItem;

@interface AFCache : NSObject {
	BOOL cacheEnabled;
	NSString *dataPath;
	NSMutableDictionary *cacheInfoStore;
	NSMutableDictionary *pendingConnections;
    NSMutableDictionary *clientItems;
	NSMutableArray		*downloadQueue;
	BOOL _offline;
	int requestCounter;
	int concurrentConnections;
	double maxItemFileSize;
	double diskCacheDisplacementTresholdSize;
	NSDictionary *suffixToMimeTypeMap;
    NSTimer* archiveTimer;
	
	BOOL downloadPermission_;
    BOOL wantsToArchive_;
	
	NetworkTimeoutIntervals networkTimeoutIntervals;
	NSMutableDictionary *packageInfos;
    
    NSOperationQueue* packageArchiveQueue_;
	BOOL failOnStatusCodeAbove400;
}


@property BOOL cacheEnabled;
@property (nonatomic, copy) NSString *dataPath;
@property (nonatomic, retain) NSMutableDictionary *cacheInfoStore;
@property (nonatomic, retain) NSMutableDictionary *pendingConnections;
@property (nonatomic, retain) NSMutableArray *downloadQueue;
@property (nonatomic, retain) NSDictionary *suffixToMimeTypeMap;
@property (nonatomic, retain) NSDictionary *clientItems;
@property (nonatomic, assign) double maxItemFileSize;
@property (nonatomic, assign) double diskCacheDisplacementTresholdSize;
@property (nonatomic, assign) int concurrentConnections;
@property BOOL downloadPermission;
@property (nonatomic, assign) NetworkTimeoutIntervals networkTimeoutIntervals;
@property (nonatomic, retain) NSMutableDictionary *packageInfos;
@property (nonatomic, assign) BOOL failOnStatusCodeAbove400;
		
+ (AFCache *)sharedInstance;


- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url
                               delegate: (id) aDelegate;

- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url
                               delegate: (id) aDelegate
                                options: (int) options;

- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url 
							   delegate: (id) aDelegate 
							   selector: (SEL) aSelector 
						didFailSelector: (SEL) aFailSelector 
								options: (int) options;

- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url 
							   delegate: (id) aDelegate 
							   selector: (SEL) aSelector 
						didFailSelector: (SEL) aFailSelector 
								options: (int) options
                               userData: (id)userData
							   username: (NSString *)aUsername
							   password: (NSString *)aPassword;

- (AFCacheableItem *)cachedObjectForURL:(NSURL *)url 
							   delegate:(id) aDelegate 
							   selector:(SEL)aSelector 
						didFailSelector:(SEL)didFailSelector 
								options: (int) options;


///////////////////////////////////////////////////////////////////////////////////////////
/*
 *
	DEPRECATED Methods - Please avoid using those, due to the omitted DidFailSelector
 *
 */

- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url
                                options: (int) options DEPRECATED_ATTRIBUTE;

- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url
							   delegate: (id) aDelegate
							   selector: (SEL) aSelector
								options: (int) options DEPRECATED_ATTRIBUTE;  

- (AFCacheableItem *)cachedObjectForURL: (NSURL *) url
							   delegate: (id) aDelegate 
                               selector: (SEL) aSelector 
								options: (int) options
							   userData: (id)userData DEPRECATED_ATTRIBUTE; 

////////////////////////////////////////////////////////////////////////////////////////

- (void)invalidateAll;
- (void)archive;
- (BOOL)isOffline;
- (void)setOffline:(BOOL)value;
- (BOOL)isConnectedToNetwork;
- (int)totalRequestsForSession;
- (int)requestsPending;
- (void)doHousekeeping;
- (BOOL)hasCachedItemForURL:(NSURL *)url;
- (unsigned long)diskCacheSize;
- (void)cancelAsynchronousOperationsForURL:(NSURL *)url itemDelegate:(id)aDelegate;
- (void)cancelAsynchronousOperationsForURL:(NSURL *)url itemDelegate:(id)aDelegate didLoadSelector:(SEL)selector;
- (void)cancelAsynchronousOperationsForDelegate:(id)aDelegate;
- (NSArray*)cacheableItemsForURL:(NSURL*)url;
- (void)flushDownloadQueue;

@end

@interface AFCache( LoggingSupport ) 

/*
 * currently ignored if not built against EngineRoom - SUBJECT TO CHANGE WITHOUT NOTICE
 */

+ (void) setLoggingEnabled: (BOOL) enabled; 
+ (void) setLogFormat: (NSString *) logFormat;

@end
/*
 *
 * Copyright 2008 Artifacts - Fine Software Development
 * http://www.artifacts.de
 * Author: Michael Markowski (m.markowski@artifacts.de)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#if TARGET_OS_IPHONE

#endif



#ifdef USE_TOUCHXML

#endif

@class AFCache;
@class AFCacheableItem;
@protocol AFCacheableItemDelegate;

enum kCacheStatus {
	kCacheStatusNew = 0,
	kCacheStatusFresh = 1, // written into cacheableitem when item is fresh, either after fetching it for the first time or by revalidation.
	kCacheStatusModified = 2, // if ims request returns status 200
	kCacheStatusNotModified = 4,
	kCacheStatusRevalidationPending = 5,
	kCacheStatusStale = 6,
	kCacheStatusDownloading = 7, // item is not fully downloaded
};

@interface AFCacheableItem : NSObject {
	NSURL *url;
	NSData *data;
	AFCache *cache;
	id <AFCacheableItemDelegate> delegate;
	BOOL persistable;
	BOOL ignoreErrors;
	SEL connectionDidFinishSelector;
	SEL connectionDidFailSelector;
	NSError *error;
	id userData;
	
	// validUntil holds the calculated expire date of the cached object.
	// It is either equal to Expires (if Expires header is set), or the date
	// based on the request time + max-age (if max-age header is set).
	// If neither Expires nor max-age is given or if the resource must not
	// be cached valitUntil is nil.	
	NSDate *validUntil;
	int cacheStatus;
	AFCacheableItemInfo *info;
	int tag; // for debugging and testing purposes
	BOOL isPackageArchive;
	uint64_t currentContentLength;
    NSFileHandle*   fileHandle;
	
	/*
	 Some data for the HTTP Basic Authentification
	 */
	NSString *username;
	NSString *password;
    
    BOOL    isRevalidating;
}

@property (nonatomic, retain) NSURL *url;

@property (nonatomic, retain) NSData *data;
@property (nonatomic, retain) AFCache *cache;
@property (nonatomic, assign) id <AFCacheableItemDelegate> delegate;
@property (nonatomic, retain) NSError *error;
@property (nonatomic, retain) NSDate *validUntil;
@property (nonatomic, assign) BOOL persistable;
@property (nonatomic, assign) BOOL ignoreErrors;
@property (nonatomic, assign) SEL connectionDidFinishSelector;
@property (nonatomic, assign) SEL connectionDidFailSelector;
@property (nonatomic, assign) int cacheStatus;
@property (nonatomic, retain) AFCacheableItemInfo *info;
@property (nonatomic, assign) id userData;
@property (nonatomic, assign) BOOL isPackageArchive;
@property (nonatomic, assign) uint64_t currentContentLength;
@property (nonatomic, retain) NSString *username;
@property (nonatomic, retain) NSString *password;

@property (nonatomic, retain) NSFileHandle* fileHandle;
@property (readonly) NSString* filePath;

@property (nonatomic, assign) BOOL isRevalidating;

- (void)connection: (NSURLConnection *) connection didReceiveData: (NSData *) data;
- (void)connectionDidFinishLoading: (NSURLConnection *) connection;
- (void)connection: (NSURLConnection *) connection didReceiveResponse: (NSURLResponse *) response;
- (void)connection: (NSURLConnection *) connection didFailWithError: (NSError *) error;
- (void)handleResponse:(NSURLResponse *)response;
- (BOOL)isFresh;
- (BOOL)isCachedOnDisk;
- (NSString*)guessContentType;
- (void)validateCacheStatus;
- (uint64_t)currentContentLength;
- (BOOL)isComplete;

- (NSString *)filename;
- (NSString *)asString;
- (NSString*)mimeType __attribute__((deprecated)); // mimeType moved to AFCacheableItemInfo. 
// This method is implicitly guessing the mimetype which might be confusing because there's a property mimeType in AFCacheableItemInfo.

#ifdef USE_TOUCHXML
- (CXMLDocument *)asXMLDocument;
#endif

@end

@protocol AFCacheableItemDelegate < NSObject >

- (void) connectionDidFail: (AFCacheableItem *) cacheableItem;
- (void) connectionDidFinish: (AFCacheableItem *) cacheableItem;

@optional
- (void) packageArchiveDidReceiveData: (AFCacheableItem *) cacheableItem;
- (void) packageArchiveDidFinishLoading: (AFCacheableItem *) cacheableItem;
- (void) packageArchiveDidFinishExtracting: (AFCacheableItem *) cacheableItem;
- (void) packageArchiveDidFailExtracting: (AFCacheableItem *) cacheableItem;
- (void) packageArchiveDidFailLoading: (AFCacheableItem *) cacheableItem;

- (void) cacheableItemDidReceiveData: (AFCacheableItem *) cacheableItem;

@end
/*
 *
 * Copyright 2008 Artifacts - Fine Software Development
 * http://www.artifacts.de
 * Contributed by Nico Schmidt - savoysoftware.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */




@interface AFURLCache : NSURLCache
{

}

@end
//
//  AFCacheableItem+MetaDescription.h
//  AFCache
//
//  Created by Michael Markowski on 16.07.10.
//  Copyright 2010 Artifacts - Fine Software Development. All rights reserved.
//




@interface AFCacheableItem (Packaging)

- (AFCacheableItem*)initWithURL:(NSURL*)URL
           lastModified:(NSDate*)lastModified 
           expireDate:(NSDate*)expireDate
          contentType:(NSString*)contentType;

- (AFCacheableItem*)initWithURL:(NSURL*)URL
				  lastModified:(NSDate*)lastModified 
					expireDate:(NSDate*)expireDate;

- (NSString*)metaDescription;
- (NSString*)metaJSON;

+ (NSString *)urlEncodeValue:(NSString *)str;
- (void)setDataAndFile:(NSData*)theData;

@end
//
//  AFPackageItemInfo.h
//  AFCache
//
//  Created by Michael Markowski on 28.01.11.
//  Copyright 2011 Artifacts - Fine Software Development. All rights reserved.
//




@interface AFPackageInfo : NSObject {
	NSURL *packageURL;
	NSURL *baseURL;
	NSArray *resourceURLs;
	NSMutableDictionary *userData;
}

@property (nonatomic, retain) NSURL *packageURL;
@property (nonatomic, retain) NSURL *baseURL;
@property (nonatomic, retain) NSArray *resourceURLs;
@property (nonatomic, retain) NSMutableDictionary *userData;

@end
//
//  AFCache+Packaging.h
//  AFCache
//
//  Created by Michael Markowski on 13.08.10.
//  Copyright 2010 Artifacts - Fine Software Development. All rights reserved.
//





@interface AFCache (Packaging)



- (BOOL)importCacheableItem:(AFCacheableItem*)cacheableItem withData:(NSData*)theData;
- (AFCacheableItem *)requestPackageArchive: (NSURL *) url delegate: (id) aDelegate;
- (AFCacheableItem *)requestPackageArchive: (NSURL *) url delegate: (id) aDelegate username: (NSString*) username password: (NSString*) password;
- (void)consumePackageArchive:(AFCacheableItem*)cacheableItem preservePackageInfo:(BOOL)preservePackageInfo;
- (void)packageArchiveDidFinishLoading: (AFCacheableItem *) cacheableItem;
- (NSString*)userDataPathForPackageArchiveKey:(NSString*)archiveKey;
- (AFPackageInfo*)packageInfoForURL:(NSURL*)url;

// wipe out a cachable item completely
- (void)purgeCacheableItemForURL:(NSURL*)url;

// remove an imported package zip
- (void)purgePackageArchiveForURL:(NSURL*)url;

// announce files residing in the urlcachestore folder by reading the cache manifest file
// this method assumes that the files already have been extracted into the urlcachestore folder
- (AFPackageInfo*)newPackageInfoByImportingCacheManifestAtPath:(NSString*)manifestPath intoCacheStoreWithPath:(NSString*)urlCacheStorePath withPackageURL:(NSURL*)packageURL;
- (void)storeCacheInfo:(NSDictionary*)dictionary;

// Deprecated methods:

#pragma mark -
#pragma mark Deprecated methods

// Deprecated. Use consumePackageArchive:preservePackageInfo: instead
- (void)consumePackageArchive:(AFCacheableItem*)cacheableItem DEPRECATED_ATTRIBUTE; 
- (void)consumePackageArchive:(AFCacheableItem*)cacheableItem preservePackageInfo:(BOOL)preservePackageInfo;
- (void)consumePackageArchive:(AFCacheableItem*)cacheableItem userData:(NSDictionary*)userData preservePackageInfo:(BOOL)preservePackageInfo;

@end
