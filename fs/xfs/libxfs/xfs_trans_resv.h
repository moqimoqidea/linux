// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2002,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#ifndef	__XFS_TRANS_RESV_H__
#define	__XFS_TRANS_RESV_H__

struct xfs_mount;

/*
 * structure for maintaining pre-calculated transaction reservations.
 */
struct xfs_trans_res {
	uint	tr_logres;	/* log space unit in bytes per log ticket */
	int	tr_logcount;	/* number of log operations per log ticket */
	int	tr_logflags;	/* log flags, currently only used for indicating
				 * a reservation request is permanent or not */
};

struct xfs_trans_resv {
	struct xfs_trans_res	tr_write;	/* extent alloc trans */
	struct xfs_trans_res	tr_itruncate;	/* truncate trans */
	struct xfs_trans_res	tr_rename;	/* rename trans */
	struct xfs_trans_res	tr_link;	/* link trans */
	struct xfs_trans_res	tr_remove;	/* unlink trans */
	struct xfs_trans_res	tr_symlink;	/* symlink trans */
	struct xfs_trans_res	tr_create;	/* create trans */
	struct xfs_trans_res	tr_create_tmpfile; /* create O_TMPFILE trans */
	struct xfs_trans_res	tr_mkdir;	/* mkdir trans */
	struct xfs_trans_res	tr_ifree;	/* inode free trans */
	struct xfs_trans_res	tr_ichange;	/* inode update trans */
	struct xfs_trans_res	tr_growdata;	/* fs data section grow trans */
	struct xfs_trans_res	tr_addafork;	/* add inode attr fork trans */
	struct xfs_trans_res	tr_writeid;	/* write setuid/setgid file */
	struct xfs_trans_res	tr_attrinval;	/* attr fork buffer
						 * invalidation */
	struct xfs_trans_res	tr_attrsetm;	/* set/create an attribute at
						 * mount time */
	struct xfs_trans_res	tr_attrsetrt;	/* set/create an attribute at
						 * runtime */
	struct xfs_trans_res	tr_attrrm;	/* remove an attribute */
	struct xfs_trans_res	tr_clearagi;	/* clear agi unlinked bucket */
	struct xfs_trans_res	tr_growrtalloc;	/* grow realtime allocations */
	struct xfs_trans_res	tr_growrtzero;	/* grow realtime zeroing */
	struct xfs_trans_res	tr_growrtfree;	/* grow realtime freeing */
	struct xfs_trans_res	tr_qm_setqlim;	/* adjust quota limits */
	struct xfs_trans_res	tr_qm_dqalloc;	/* allocate quota on disk */
	struct xfs_trans_res	tr_sb;		/* modify superblock */
	struct xfs_trans_res	tr_fsyncts;	/* update timestamps on fsync */
	struct xfs_trans_res	tr_atomic_ioend; /* untorn write completion */
};

/* shorthand way of accessing reservation structure */
#define M_RES(mp)	(&(mp)->m_resv)

/*
 * Per-directory log reservation for any directory change.
 * dir blocks: (1 btree block per level + data block + free block) * dblock size
 * bmap btree: (levels + 2) * max depth * block size
 * v2 directory blocks can be fragmented below the dirblksize down to the fsb
 * size, so account for that in the DAENTER macros.
 */
#define	XFS_DIROP_LOG_RES(mp)	\
	(XFS_FSB_TO_B(mp, XFS_DAENTER_BLOCKS(mp, XFS_DATA_FORK)) + \
	 (XFS_FSB_TO_B(mp, XFS_DAENTER_BMAPS(mp, XFS_DATA_FORK) + 1)))
#define	XFS_DIROP_LOG_COUNT(mp)	\
	(XFS_DAENTER_BLOCKS(mp, XFS_DATA_FORK) + \
	 XFS_DAENTER_BMAPS(mp, XFS_DATA_FORK) + 1)

/*
 * Various log count values.
 */
#define	XFS_DEFAULT_LOG_COUNT		1
#define	XFS_DEFAULT_PERM_LOG_COUNT	2
#define	XFS_ITRUNCATE_LOG_COUNT		2
#define XFS_INACTIVE_LOG_COUNT		2
#define	XFS_CREATE_LOG_COUNT		2
#define	XFS_CREATE_TMPFILE_LOG_COUNT	2
#define	XFS_MKDIR_LOG_COUNT		3
#define	XFS_SYMLINK_LOG_COUNT		3
#define	XFS_REMOVE_LOG_COUNT		2
#define	XFS_LINK_LOG_COUNT		2
#define	XFS_RENAME_LOG_COUNT		2
#define	XFS_WRITE_LOG_COUNT		2
#define	XFS_ADDAFORK_LOG_COUNT		2
#define	XFS_ATTRINVAL_LOG_COUNT		1
#define	XFS_ATTRSET_LOG_COUNT		3
#define	XFS_ATTRRM_LOG_COUNT		3

/*
 * Original log operation counts were overestimated in the early days of
 * reflink.  These are retained here purely for minimum log size calculations
 * and must not be used for runtime reservations.
 */
#define	XFS_ITRUNCATE_LOG_COUNT_REFLINK	8
#define	XFS_WRITE_LOG_COUNT_REFLINK	8

void xfs_trans_resv_calc(struct xfs_mount *mp, struct xfs_trans_resv *resp);
uint xfs_allocfree_block_count(struct xfs_mount *mp, uint num_ops);

unsigned int xfs_calc_finish_bui_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);

unsigned int xfs_calc_finish_efi_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);
unsigned int xfs_calc_finish_rt_efi_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);

unsigned int xfs_calc_finish_rui_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);
unsigned int xfs_calc_finish_rt_rui_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);

unsigned int xfs_calc_finish_cui_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);
unsigned int xfs_calc_finish_rt_cui_reservation(struct xfs_mount *mp,
		unsigned int nr_ops);

unsigned int xfs_calc_itruncate_reservation_minlogsize(struct xfs_mount *mp);
unsigned int xfs_calc_write_reservation_minlogsize(struct xfs_mount *mp);
unsigned int xfs_calc_qm_dqalloc_reservation_minlogsize(struct xfs_mount *mp);

xfs_extlen_t xfs_calc_max_atomic_write_fsblocks(struct xfs_mount *mp);
xfs_extlen_t xfs_calc_atomic_write_log_geometry(struct xfs_mount *mp,
		xfs_extlen_t blockcount, unsigned int *new_logres);
int xfs_calc_atomic_write_reservation(struct xfs_mount *mp,
		xfs_extlen_t blockcount);

#endif	/* __XFS_TRANS_RESV_H__ */
