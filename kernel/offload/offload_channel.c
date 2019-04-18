#include "console.h"
#include "offload_channel.h"
#include "offload_memory_config.h"
#include "page.h"

static int g_offload_initialization_flag;
static channel_t *g_offload_channels;
static int g_n_offload_channels;

/**
 * @brief initialize circular queue
 * @param cq pointer to circular queue
 * @param size the size of circular queue
 * @return none
 */
void cq_init(struct circular_queue *cq, unsigned long size)
{
  cq->head = 0;
  cq->tail = 0;
  cq->size = size;
}

/**
 * @brief return the aviilable size of circular queue
 * @param cq pointer to circular queue
 * @return available data size
 */
int cq_avail_data(struct circular_queue *cq)
{
  return (cq->head - cq->tail) & (cq->size - 1);
}

/**
 * @brief return the free space size of circular queue
 * @param cq pointer to circular queue
 * @return free space size
 */
int cq_free_space(struct circular_queue *cq)
{
  return (cq->size - 1 - cq_avail_data(cq));
}

/**
 * @brief return circular queue is full or not
 * @param cq pointer to circular queue
 * @return  if full 1 else 0
 */
int cq_is_full(struct circular_queue *cq)
{
  return ((cq->tail + 1) % cq->size == cq->head);
}

/**
 * @brief return circular queue is empty or not
 * @param cq pointer to circular queue
 * @return  if empty 1 else 0
 */
int cq_is_empty(struct circular_queue *cq)
{
  return (cq->head == cq->tail);
}

/**
 * @brief add data to circular cueue
 * Call this function after confirming enough room is available
 * @param cq pointer to circular queue
 * @param data data to add
 * @return (1)
 */
int cq_add_data(struct circular_queue *cq, char data)
{
  while (cq_free_space(cq) == 0);
  (cq->data + cq->head)->d[0] = data;
//    mbarrier();
  cq->head = (cq->head + 1) % cq->size;

  return 1;
}

/**
 * @brief remove data from circular queue
 * Call this function after confirming requested size of data is available
 * @param cq pointer to circular queue
 * @param data data to remove
 * @return (1)
 */
int cq_remove_data(struct circular_queue *cq, char *data)
{
  while (cq_avail_data(cq) == 0);
  *data = (cq->data + cq->tail)->d[0];
//    mbarrier();
  cq->tail = (cq->tail + 1) % cq->size;

  return 1;
}

/**
 * @brief initialize io offload
 * @return success (TRUE), fail (FALSE)
 */
BOOL init_offload_channel()
{
	volatile QWORD offload_magic = 0;

	channel_t *cur_channel;
	int offload_channels_offset = 0;

	QWORD n_icq = 0;
	QWORD n_ocq = 0;
	QWORD icq_base_va = 0;
	QWORD ocq_base_va = 0;
	QWORD offload_channel_info_va = 0;
	QWORD offload_channel_base_va = 0;
 
	g_offload_channels = (channel_t *) ((QWORD) OFFLOAD_CHANNEL_STRUCT_VA);

	// set io offload channel info va
	offload_channel_info_va = (QWORD) (OFFLOAD_CHANNEL_INFO_VA);
	offload_channel_base_va = (QWORD) (OFFLOAD_CHANNEL_BASE_VA);

	offload_magic = *((QWORD *) offload_channel_info_va);
	// check offload channel is initialized or not
	if (offload_magic != (QWORD) OFFLOAD_MAGIC) {
                g_offload_initialization_flag = FALSE;
                return (FALSE);
        }
        else {
                g_offload_initialization_flag = TRUE;
        }

	g_n_offload_channels = * ((QWORD *)(offload_channel_info_va) + 1);
	n_icq = *((QWORD *)(offload_channel_info_va) + 2);
	n_ocq = *((QWORD *)(offload_channel_info_va) + 3);

	// initialize offload channel
	for(offload_channels_offset = 0; offload_channels_offset < g_n_offload_channels; offload_channels_offset++) {

		icq_base_va = (QWORD) offload_channel_base_va + offload_channels_offset * (n_icq + n_ocq) * PAGE_SIZE_4K;
		// map icq of ith channel
		cur_channel = &g_offload_channels[offload_channels_offset];

		cur_channel->in = (struct circular_queue *) (icq_base_va);

		// map ocq of ith channel
		ocq_base_va = (QWORD) icq_base_va + (n_icq * PAGE_SIZE_4K);
		cur_channel->out = (struct circular_queue *) (ocq_base_va);
	}
	return(TRUE);
}

/**
 * @brief get offload channel
 * channel: 0 ~ (OFFLOAD_MAX_CHANNEL-1)
 * @param n_requested_channel channel number to use
 * @return success (pointer to channel), fail (NULL)
 */
channel_t *get_offload_channel(int n_requested_channel)
{
  int offload_channels_offset = 0;

  // offload channel is not initialized
  if(g_offload_initialization_flag == FALSE)
	return (NULL);

  offload_channels_offset = (n_requested_channel == -1) ? get_apic_id() : n_requested_channel;

  offload_channels_offset = (int) (offload_channels_offset % g_n_offload_channels);

  return (&(g_offload_channels[offload_channels_offset]));
}
