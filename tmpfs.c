#include <u.h>
#include <libc.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>

struct Player {
	int team;
	int id;
	char *name;
	struct Player *next;
};

static char Einvalid[] = "invalid control";
static char Ebig[] = "too big an input";
static char Eperm[] = "permission denied";

static struct {
	File *add;
	File *rm;
	File *list;
} ctl;

static struct Player player = {
	0,
	0,
	"",
	&player
};

int
threadmaybackground(void)
{
	return 1;
}

static void
createplayer(void)
{
	int i;
	struct Player **node;

	node = &player.next;
	for(i = 1; i <= 32; i++)
	{
		*node = emalloc9p(sizeof **node);
		(*node)->team = 0;
		(*node)->id = i;
		(*node)->name = estrdup9p("");
		node = &(*node)->next;
	}
	*node = &player;
}

static void
fsread(Req *r)
{
	respond(r, nil);
}

static void
fswrite(Req *r)
{
	respond(r, nil);
}

static void
fscreate(Req *r)
{
	respond(r, Eperm);
}

static void
fsremove(Req *r)
{
	respond(r, Eperm);
}

static void
fswstat(Req *r)
{
	respond(r, nil);
}

static Srv fs = {
	.read=		fsread,
	.write=		fswrite,
	.create=	fscreate,
	.remove=	fsremove,
	.wstat=		fswstat
};

void
threadmain(int argc, char *argv[])
{
	char *srvname;

	if(argc != 1)
		return;
	fs.tree = alloctree(nil, nil, 0777 | DMDIR, nil);
	createplayer();
	ctl.add = createfile(fs.tree->root, "add", nil, 0660, nil);
	ctl.rm = createfile(fs.tree->root, "remove", nil, 0660, nil);
	ctl.list = createfile(fs.tree->root, "list", nil, 0664, nil);
	srvname = "rcon_balancer";
	threadpostmountsrv(&fs, srvname, nil, MBEFORE);
	threadexits(nil);
}
