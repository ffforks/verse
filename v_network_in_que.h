typedef struct{
	void	*oldest;
	void	*newest;
	uint32	packet_id;
	uint32	seconds;
}VNetInQueue;

extern void v_niq_clear(VNetInQueue *queue);
extern char *v_niq_get(VNetInQueue *queue, size_t *length);
extern char *v_niq_store(VNetInQueue *queue, size_t length, unsigned int packet_id);

extern boolean v_niq_time_out(const VNetInQueue *queue);
