#ifndef _FS_CEPH_CEPH_H
#define _FS_CEPH_CEPH_H

#include <linux/ceph_fs.h>
#include <linux/fs.h>

#include "client.h"

/*
 * mount options
 */
#define CEPH_MOUNT_FSID     1
#define CEPH_MOUNT_NOSHARE  2  /* don't share client with other mounts */

struct ceph_mount_args {
	int mntflags;
	int flags;
	struct ceph_fsid fsid;
	int num_mon;
	struct ceph_entity_addr mon_addr[5];
	int mon_port;
	char path[100];
};


/*
 * CEPH per-mount superblock info
 */
struct ceph_super_info {
	struct ceph_mount_args mount_args;
	struct ceph_client *sb_client;
	
	/* FIXME: ptr to inode of my relative offset into the filesystem,
	   so we can appropriately mangle/adjust path names in requests, etc...? */
};

static inline struct ceph_super_info *ceph_sbinfo(struct super_block *sb)
{
	return sb->s_fs_info;
}

/*
 * CEPH file system in-core inode info
 */
struct ceph_inode_cap {
	int mds;
	int caps;
	__u64 seq;
	int flags;  /* stale, etc.? */
};	
struct ceph_inode_frag_map_item {
	__u32 frag;
	__u32 mds;
};

struct ceph_inode_info {
	struct ceph_file_layout i_layout;

	int i_dir_auth;
	struct ceph_frag_tree_head *i_fragtree, i_fragtree_static;
	int i_frag_map_nr;
	struct ceph_inode_frag_map_item *i_frag_map;
	
	int i_nr_caps;
	struct ceph_inode_cap *i_caps;
	struct ceph_inode_cap i_caps_static[2];
	off_t i_wr_size;
	struct ceph_timeval i_wr_mtime;
	
	struct inode vfs_inode;
};

static inline struct ceph_inode_info *CEPH_I(struct inode *inode)
{
	return list_entry(inode, struct ceph_inode_info, vfs_inode);
}



/* file.c */
extern const struct inode_operations ceph_file_inops;
extern const struct file_operations ceph_file_operations;
extern const struct address_space_operations ceph_aops;

/* dir.c */
extern const struct inode_operations ceph_dir_inops;
extern const struct file_operations ceph_dir_operations;

#endif /* _FS_CEPH_CEPH_H */
