//
//  mAExampleBrowser.m
//  miniAudicle
//
//  Created by Spencer Salazar on 6/13/13.
//
//

#import "mAExampleBrowser.h"
#import "miniAudicleDocument.h"
#import "miniAudicleController.h"


@interface NSFileManager (isDirectory)

- (BOOL)isDirectory:(NSString *)path;

@end

@implementation NSFileManager (isDirectory)

- (BOOL)isDirectory:(NSString *)path
{
    BOOL isDirectory = NO;
    if([self fileExistsAtPath:path isDirectory:&isDirectory])
        return isDirectory;
    else
        return NO;
}


@end


@interface mAExampleBrowser ()

@end

@implementation mAExampleBrowser

- (id)initWithWindow:(NSWindow *)window
{
    self = [super initWithWindow:window];
    if (self)
    {
        // Initialization code here.
    }
    
    return self;
}

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
    
    [_browser setDoubleAction:@selector(open:)];
    [_browser setTarget:self];
    [_openButton setEnabled:NO];
}

- (void)flagsChanged:(NSEvent *)theEvent
{
//    if([theEvent modifierFlags] & NSAlternateKeyMask)
//        [_openButton setTitle:@"Open in Tab"];
//    else
//        [_openButton setTitle:@"Open"];
    
    [super flagsChanged:theEvent];
}


#pragma mark NSWindowDelegate

- (void)windowWillClose:(NSNotification *)notification
{
    // clear selection
    [_browser selectRowIndexes:[NSIndexSet indexSet] inColumn:[_browser selectedColumn]];
    // disable open button
    [_openButton setEnabled:NO];
}


#pragma mark IBActions

- (IBAction)open:(id)sender
{
    NSString * examplePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"examples"];
    NSString * columnPath = [examplePath stringByAppendingFormat:@"/%@", [_browser pathToColumn:[_browser selectedColumn]]];
    
//    BOOL inTab = [NSEvent modifierFlags] & NSAlternateKeyMask;
    miniAudicleController * controller = (miniAudicleController *)[NSDocumentController sharedDocumentController];
    
    for(NSBrowserCell * cell in [_browser selectedCells])
    {
        NSString * filePath = [columnPath stringByAppendingPathComponent:[cell title]];
        miniAudicleDocument * doc;
        
//        if(inTab)
//        {
//            doc = [controller openDocumentWithContentsOfURL:[NSURL fileURLWithPath:filePath]
//                                                    display:YES
//                                                      error:nil
//                                                      inTab:YES];
//        }
//        else
        {
            doc = [controller openDocumentWithContentsOfURL:[NSURL fileURLWithPath:filePath]
                                                    display:YES
                                                      error:nil];
        }
        
        doc.readOnly = YES;
    }
    
    [self.window close];
}

- (IBAction)cancel:(id)sender
{
    [self.window close];
}

- (IBAction)select:(id)sender
{
    BOOL enable = NO;
    
    for(NSBrowserCell * cell in [_browser selectedCells])
    {
        if([cell isLeaf])
        {
            enable = YES;
            break;
        }
    }
    
    [_openButton setEnabled:enable];
}


#pragma mark NSBrowserDelegate

- (NSInteger)browser:(NSBrowser *)sender numberOfRowsInColumn:(NSInteger)column
{
    NSFileManager * fileManager = [NSFileManager defaultManager];
    
    NSString * examplePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"examples"];
    NSString * columnPath = [examplePath stringByAppendingFormat:@"/%@", [sender pathToColumn:column]];
    NSArray * contents = [fileManager contentsOfDirectoryAtPath:columnPath error:nil];
    
    return [contents count];
}

- (void)browser:(NSBrowser *)sender willDisplayCell:(id)cell atRow:(NSInteger)row column:(NSInteger)column
{
    NSBundle * mainBundle = [NSBundle mainBundle];
    NSFileManager * fileManager = [NSFileManager defaultManager];

    NSString * examplePath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"examples"];
    NSString * columnPath = [examplePath stringByAppendingFormat:@"/%@", [sender pathToColumn:column]];
    NSArray * files = [fileManager contentsOfDirectoryAtPath:columnPath error:nil];
    NSString * file = [files objectAtIndex:row];
    NSString * fullpath = [columnPath stringByAppendingPathComponent:file];
    
    [cell setTitle:file];
    [cell setEnabled:YES];
    [cell setImage:nil];

    if([fileManager isDirectory:fullpath])
    {
        [cell setImage:[mainBundle imageForResource:@"folder.png"]];
        [cell setLeaf:NO];
    }
    else
    {
        if([[file pathExtension] isEqualToString:@"ck"])
            [cell setImage:[mainBundle imageForResource:@"ckmini.png"]];
        else
            [cell setEnabled:NO];
        [cell setLeaf:YES];
    }
}

- (CGFloat)browser:(NSBrowser *)browser heightOfRow:(NSInteger)row inColumn:(NSInteger)columnIndex
{
    return 64;
}

@end