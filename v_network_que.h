/*
**
*/

typedef struct VNetQueue VNetQueue;

extern VNetQueue * v_nq_create_network_queue(void);
extern void	v_nq_destroy_network_queue(VNetQueue *queue);
extern void	v_nq_send_buf(VNetQueue *queue, VCMDBufHead *buf);
extern void	v_nq_send_ack_nak_buf(VNetQueue *queue, VCMDBufHead *buf);

extern void	v_nq_send_ack(VNetQueue *queue, unsigned int id);
extern void	v_nq_send_nak(VNetQueue *queue, unsigned int id);

extern char *	v_nq_get_packed(VNetQueue *queue, unsigned int *length);
extern void	v_nq_store_packed(VNetQueue *queue, const char *data, unsigned int length);

extern boolean	v_nq_send_queue(VNetQueue *queue, void *address);
