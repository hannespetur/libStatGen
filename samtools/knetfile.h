#ifndef KNETFILE_H
#define KNETFILE_H

#include <stdint.h>
#include <fcntl.h>

#ifndef _WIN32
#define netread(fd, ptr, len) read(fd, ptr, len)
#define netwrite(fd, ptr, len) write(fd, ptr, len)
#define netclose(fd) close(fd)
#else
#include <winsock2.h>
#define netread(fd, ptr, len) recv(fd, ptr, len, 0)
#define netwrite(fd, ptr, len) send(fd, ptr, len, 0)
#define netclose(fd) closesocket(fd)
#endif

// FIXME: currently I/O is unbuffered

#define KNF_TYPE_LOCAL 1
#define KNF_TYPE_FTP   2
#define KNF_TYPE_HTTP  3

typedef struct knetFile_s {
	int type, fd;
	int64_t offset;
	char *host, *port;

	// the following are for FTP only
	int ctrl_fd, pasv_ip[4], pasv_port, max_response, no_reconnect, is_ready;
	char *response, *retr, *size_cmd;
	int64_t seek_offset; // for lazy seek
    int64_t file_size;

	// the following are for HTTP only
	char *path, *http_host;
} knetFile;

#define knet_tell(fp) ((fp)->offset)
#define knet_fileno(fp) ((fp)->fd)

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
	int knet_win32_init();
	void knet_win32_destroy();
#endif

        // Pass in non-zero to make knetfile silent (no messages), pass in 
        // 0 to keep any messages (default is 0).
        void knet_silent(int silent);

	knetFile *knet_open2(const char *fn, const char *mode);

	/* 
	   This only works with local files.
	 */
	knetFile *knet_dopen2(int fd, const char *mode);

	/*
	  If ->is_ready==0, this routine updates ->fd; otherwise, it simply
	  reads from ->fd.
	 */
	ssize_t knet_read2(knetFile *fp, void *buf, size_t len);

	/*
	  This routine only sets ->offset and ->is_ready=0. It does not
	  communicate with the FTP server.
	 */
	off_t knet_seek2(knetFile *fp, off_t off, int whence);
	int knet_close2(knetFile *fp);

#ifdef __cplusplus
}
#endif

#endif
