package modules.node.cache;

moduleinterface rcache
{
    parameters:
	int C;
    gates:
	inout rcache_port;
}

simple rbase_cache like rcache
{
    parameters:
    	@display("i=block/buffer2;is=l");

	string DS = default("lce");
	int C = default (100);

    gates:
	inout rcache_port;
}



simple rrandom_cache extends rbase_cache{
    @class(random_cache);
}

simple rlru_cache extends rbase_cache{
    @class(lru_cache);
}

simple rtwo_cache extends rbase_cache{
    @class(two_cache);
}

simple rfifo_cache extends rbase_cache{
    @class(fifo_cache);
}

