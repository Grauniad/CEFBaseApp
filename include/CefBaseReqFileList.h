/*
 * CefBaseReqFileList.h
 *
 *  Created on: 5 Apr 2015
 *      Author: lhumphreys
 */

#ifndef CEFBASEREQFILELIST_H_
#define CEFBASEREQFILELIST_H_

#include <CefBaseRequestReplies.h>
#include <SimpleJSON.h>

/*
 * Request the list of files matching a glob pattern.
 *
 * REQUEST:
 *    {
 *         pattern: "*.out",
 *         prefix: ""
 *    }
 *
 * REPLY:
 *    {
 *         files: [
 *            "file1.out",
 *            "file2.out"
 *         ]
 *    }
 *
 *
 * WARNING: The use of this query has obvious security issues, however it is
 *           assumed the user already has access to the PC, and that this code
 *           is not running on a web server...
 */
class CefBaseReqFileList: public CefBaseJSRequestReply {
public:
    CefBaseReqFileList();

    virtual std::string OnRequest(RequestContext& context);

    virtual ~CefBaseReqFileList();
private:
    NewStringField(pattern);
    NewStringField(prefix);
    NewStringArrayField(files);
    typedef SimpleParsedJSON<pattern,prefix> Request;
    typedef SimpleParsedJSON<files> Reply;

    Request request;
    Reply   reply;
};

#endif /* CEFBASEREQFILELIST_H_ */
