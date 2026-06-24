function findIndex(nodeId) {
  const list = GraphStore.nodeList;
  for (let i = 0; i < list.count; i++) {
    const item = list.get(i);
    if (item.itemId === nodeId) {
      return i;
    }
  }
  return -1;
}
function findNode(nodeId) {
  const index = findIndex(nodeId);
  if (index === -1) return undefined;
  return GraphStore.nodeList.get(index);
}
