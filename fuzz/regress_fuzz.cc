#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <event2/tag.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/buffer_compat.h>
#include <event2/bufferevent.h>


static int
evtag_fuzz(const uint8_t *data, size_t size)
{
	struct evbuffer *tmp = evbuffer_new();
	struct timeval tv;

	int not_failed = 0;

	evtag_init();

	 {
		evbuffer_drain(tmp, -1);
		evbuffer_add(tmp, data, size);
		if (evtag_unmarshal_timeval(tmp, 0, &tv) != -1)
			not_failed++;
	}

    if (not_failed > 10) {
        return EXIT_FAILURE;
    }

	/* Now insert some corruption into the tag length field */
	evbuffer_drain(tmp, -1);
	evutil_timerclear(&tv);
	tv.tv_sec = 1;
	evtag_marshal_timeval(tmp, 0, &tv);
	evbuffer_add(tmp, data, size);

	((char *)EVBUFFER_DATA(tmp))[1] = '\xff';
	if (evtag_unmarshal_timeval(tmp, 0, &tv) != -1) {
		printf("evtag_unmarshal_timeval should have failed");
	}

	evbuffer_free(tmp);
    return EXIT_FAILURE;
}


extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{   
    int r = evtag_fuzz(data, size);
    if (EXIT_FAILURE == r) {
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}

