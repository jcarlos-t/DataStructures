#include <iostream>
#include <vector>

using namespace::std;

template<typename data_type>
struct PersistentQueue {
  struct StackNode {
    data_type data;
    StackNode* next;
  };

  struct Ver {
    StackNode *front;
    StackNode *back;
  };

  vector<Ver*> version_roots;

  PersistentQueue() {
    version_roots.push_back(new Ver(nullptr, nullptr));
  }

  void update_push(int version, data_type data) {
    version_roots.emplace_back(new Ver(version_roots[version]->front, 
        new StackNode(data, version_roots[version]->back)));
  }

  void update_pop(int version) {
    if (version_roots[version] -> front != nullptr) {
      cout<<version_roots[version]->front->data<<"\n";
      version_roots.emplace_back(new Ver(version_roots[version]->front->next,
            version_roots[version]->back));
    } else {
      StackNode *newfront = reverse(version_roots[version]->back);
      version_roots.emplace_back(new Ver(newfront->next, nullptr));
      cout<<newfront->data<<"\n";
    }
  }

  StackNode* reverse(StackNode *back) {
    StackNode *result = nullptr;
    StackNode *temp = back;
    while (temp != nullptr) {
      result = new StackNode(temp->data, result);
      temp = temp -> next;
    }
    return result;
  }

  data_type top(int version) {
    if (version_roots[version] == nullptr)
      return data_type(0);
    return version_roots[version]->front == nullptr ? data_type(0) : version_roots[version] -> front -> data;
  }
 };


int main() {
  PersistentQueue<int> q;
  int n; cin>>n;
  for (int i = 1; i <= n; i++) {
    int op, v, x;
    cin>>op;
    if (op == 1) {
      cin>>v; cin>>x;
      q.update_push(v, x);
    } else {
      cin>>v;
      q.update_pop(v);
    }
  }
  return 0;
}
