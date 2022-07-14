#include <stdio.h>
#include <glib.h>

typedef struct QMPRequest {
	int data;
} QMPRequest;

/* gcc  queue.c `pkg-config --cflags --libs glib-2.0` */
int main(int argc, char *argv[])
{
	GQueue *qmp_requests;
    QMPRequest *req_obj0, *req_obj1, *tmp_req_obj;

	/* 创建一个队列 */
	qmp_requests = g_queue_new();

	/* 构造元素 */
    req_obj0 = g_new0(QMPRequest, 1);
	req_obj0->data = 911;

    req_obj1 = g_new0(QMPRequest, 1);
	req_obj1->data = 922;

	/* 添加元素到队列中 */
	g_queue_push_tail(qmp_requests, req_obj0);
	g_queue_push_tail(qmp_requests, req_obj1);

	while (!g_queue_is_empty(qmp_requests)) {
		tmp_req_obj = g_queue_pop_head(qmp_requests);
		printf("req = %d\n", tmp_req_obj->data);
	}

	/* 销毁队列 */
	g_queue_free(qmp_requests);

	return 0;
}
