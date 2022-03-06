# enigma

```javascript --run
console.log('Hello, World!');
```

{% for image in rapport/jpg/ %}
  ![image]({{ rapport/jpg/ }} 'image')
{% endfor %}

{% for image in rapport/jpg/ %}
  ![image]({{ ./rapport/jpg/ }} 'image')
{% endfor %}
