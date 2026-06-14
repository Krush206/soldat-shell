#include <u.h>
#include <libc.h>
#include <fcall.h>
#include <thread.h>
#include <9p.h>

struct Team {
	File *entry;
	char *name;
	struct Team *next;
};

static File *ctl;

static char Einvalid[] = "invalid control";
static char Ebig[] = "too big an input";
static char Eperm[] = "permission denied";

static struct Team alpha = {
	nil,
	"",
	&alpha
};
static struct Team bravo = {
	nil,
	"",
	&bravo
};

int
threadmaybackground(void)
{
	return 1;
}

static void
createalpha(File *root)
{
	int i;
	struct Team **node;
	File *dir;

	dir = createfile(root, "alpha", nil, 0664 | DMDIR, nil);
	node = &alpha.next;
	for(i = 1; i <= 32; i++)
	{
		char id[8];

		*node = emalloc9p(sizeof **node);
		snprint(id, sizeof id, "%d", i);
		(*node)->entry = createfile(dir, id, nil, 0664, nil);
		(*node)->name = estrdup9p("");
		node = &(*node)->next;
	}
	*node = &alpha;
}

static void
createbravo(File *root)
{
	int i;
	struct Team **node;
	File *dir;

	dir = createfile(root, "bravo", nil, 0664 | DMDIR, nil);
	node = &bravo.next;
	for(i = 1; i <= 32; i++)
	{
		char id[8];

		*node = emalloc9p(sizeof **node);
		snprint(id, sizeof id, "%d", i);
		(*node)->entry = createfile(dir, id, nil, 0664, nil);
		(*node)->name = estrdup9p("");
		node = &(*node)->next;
	}
	*node = &bravo;
}

static int
command(char *cmd)
{
	char *ptr;

	ptr = strchr(cmd, ' ');
	if(ptr == nil)
		return 1;
	if(strncmp(cmd, "/add1", ptr - cmd) == 0)
	{
		addalpha(ptr + 1);
		return 0;
	}
	if(strncmp(cmd, "/add2", ptr - cmd) == 0)
	{
		addbravo(ptr + 1);
		return 0;
	}
	return 1;
}

static void
fsread(Req *r)
{
	respond(r, nil);
}

static void
fswrite(Req *r)
{
	File *f;
	char cmd[64];

	f = r->fid->file;
	if(f != ctl)
	{
		respond(r, Eperm);
		return;
	}
	if(r->ifcall.count >= sizeof cmd)
	{
		respond(r, Ebig);
		return;
	}
	strncpy(cmd, r->ifcall.data, r->ifcall.count);
	cmd[r->ifcall.count] = '\0';
	r->ofcall.count = r->ifcall.count;
	command(cmd);
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
	createalpha(fs.tree->root);
	createbravo(fs.tree->root);
	ctl = createfile(fs.tree->root, "ctl", nil, 0660, nil);
	srvname = "rcon_balancer";
	threadpostmountsrv(&fs, srvname, nil, MBEFORE);
	threadexits(nil);
}
