.macro  sum from=0, to=5
	.long   \from
	.if     \to-\from
	sum     "(\from+1)",\to
	.endif
	.endm
#define X 1
.macro  altsum from=0, to=6
	.long   \from
	.if     \to-\from
	sum     "(\from+1)",\to
	.endif
	.endm
